#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

#define CAM_RES0                0x00
#define CAM_RES1                0x01
#define CAM_RES2                0x02
#define CAM_RES0_WIDTH          1280
#define CAM_RES0_HEIGHT         800
#define CAM_RES1_WIDTH          640
#define CAM_RES1_HEIGHT         400
#define CAM_RES2_WIDTH          316
#define CAM_RES2_HEIGHT         208
#define CAM_RES3_WIDTH          (CAM_RES2_WIDTH+320)
#define CAM_RES3_HEIGHT         (CAM_RES2_HEIGHT/2)

#define EQ_MEM_SIZE         4000
#define EQ_HSCAN_LINE_START 0xff00
#define EQ_VSCAN_LINE_START 0xff01
#define EQ_ERROR            0xfffe
#define EQ_FRAME_END        0xffff
#define EQ_NEGATIVE         0x8000

#define EQ_STREAM_EDGES     0x0001
#define EQ_STREAM_LINES     0x0002

#define EQ_NODE_SIZE        12

#define EQ_NODE_FLAG_HLINE  0x0400
#define EQ_NODE_FLAG_VLINE  0x0800

#define LINE_BUFSIZE    CAM_RES3_WIDTH/2

#define LINE_GRID_WIDTH_REDUCTION         3
#define LINE_GRID_HEIGHT_REDUCTION        1
#define LINE_GRID_WIDTH                   (CAM_RES3_WIDTH>>LINE_GRID_WIDTH_REDUCTION)
#define LINE_GRID_HEIGHT                  (CAM_RES3_HEIGHT>>LINE_GRID_HEIGHT_REDUCTION)
#define LINE_GRID_INDEX(x, y)             (LINE_GRID_WIDTH*(y)+(x))
#define LINE_GRID_INDEX_P(p)              LINE_GRID_INDEX(p.m_x, p.m_y)
#define LINE_GRID_LINE(x, y)              (LINE_GRID(x, y)&LINE_NODE_LINE_MASK)
#define LINE_GRID_LINE_P(p)               (LINE_GRID_P(p)&LINE_NODE_LINE_MASK)
#define LINE_NODE_FLAG_HLINE              EQ_NODE_FLAG_HLINE
#define LINE_NODE_FLAG_VLINE              EQ_NODE_FLAG_VLINE
#define LINE_VSIZE                        (CAM_RES3_WIDTH/4)

typedef uint16_t LineGridNode;

// malloc(79*52*sizeof(uint16_t)...)
static uint8_t g_lineGridMem[LINE_GRID_WIDTH*LINE_GRID_HEIGHT*sizeof(LineGridNode)+8]; // +8 for extra memory at the end because little overruns sometimes happen
LineGridNode *g_lineGrid = (LineGridNode *)(g_lineGridMem);

static uint16_t g_lineBuf[LINE_BUFSIZE];

static uint16_t edges[EQ_MEM_SIZE];
static uint16_t edges_write_index = 0;
static uint16_t edges_read_index = 0;
static uint16_t edges_produced = 0;
static uint16_t edges_consumed = 0;

static uint16_t g_dist = 4;
static uint16_t g_thresh = 47;         // 20
static uint16_t g_hThresh = 47*3/5;    // 20

static uint16_t g_minLineWidth = 0;
static uint16_t g_maxLineWidth = 100;

static uint16_t g_minBarcodeLineWidth = 32;
static uint16_t g_maxBarcodeLineWidth = 56;

#define ENQUEUE_START() \
    uint16_t *data = edges; \
    uint16_t writeIndex = edges_write_index; \
    uint16_t produced = 0 

#define ENQUEUE(val) \
    data[writeIndex++] = val; \
    produced++; \
    if (writeIndex==EQ_MEM_SIZE) \
        writeIndex = 0 

#define ENQUEUE_END() \
    edges_write_index = writeIndex; \
    edges_produced += produced 

int loadEdgesFromFile(FILE *fp);
int markEdgesOnFile(FILE *fp);

int hScan(uint8_t *memy);

uint32_t readLine(uint16_t *mem, uint32_t size, bool *eof, bool *error);

int line_hLine(uint8_t row, uint16_t *buf, uint32_t len);
int line_vLine(uint8_t row, uint8_t *vstate, uint16_t *buf, uint32_t len);

void detectCodes(uint8_t row, uint16_t *buf, uint32_t len);
int markBarcodesOnFile(FILE *fp);
void clusterCodes();

struct Pointu16
{
    uint16_t x;
    uint16_t y;
};

#define BARCODELINES_SIZE           8               // arbitrary
#define BARCODELINE_POINTS_SIZE     6               // arbitrary

static uint16_t g_maxBarcodeLineJitter = 4;
static uint8_t g_blLastValid = 0;                   // barcodeLinesLastValid
struct BarcodeLine
{
    uint16_t x  = 0;
    uint16_t y = 0;
    int32_t a = 0;
    int32_t b = 0;
    uint16_t width = 0;
    uint8_t pointsIndex = 0;
    Pointu16 points[BARCODELINE_POINTS_SIZE];
};
static BarcodeLine g_barcodeLines[BARCODELINES_SIZE];


struct BarcodeCandidate
{
    uint16_t y;
    uint16_t distance;
    uint8_t count;
};

#define BARCODECANDIDATES_SIZE      4                                   // arbitrary

static BarcodeCandidate g_barcodeCandidates[BARCODECANDIDATES_SIZE];    // arbitrary
static uint8_t g_bcIndex = 0;                                             // barcodeCandidateIndex

static bool g_b3Detected = false;                                         // barcode with 3 lines detected
static bool g_b4Detected = false;                                         // barcode with 4 lines detected


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("No filename given!\n");
        return -1;
    }

    FILE *fp_edgesdata = fopen("green_bytes_only", "rb");
    if (loadEdgesFromFile(fp_edgesdata))
    {
        fclose(fp_edgesdata);
        return -1;
    }
    fclose(fp_edgesdata);

    FILE *fp_image = fopen(argv[1], "r+b");
    if (markEdgesOnFile(fp_image))
    {
        fclose(fp_image);
        return -1;
    }
    fclose(fp_image);

    uint8_t maxx[2] = {0xf0, 0xf0};

/* Filling grid start */

    uint32_t i;
    uint32_t len, tlen;
    bool eof, error;
    int8_t row;
    uint8_t vstate[LINE_VSIZE];

    for (i=0, row=-1, tlen=0; true; i++)
    {
        int32_t timer = 0;
        while((len=readLine(g_lineBuf, LINE_BUFSIZE, &eof, &error))==0)
        {    
            ++timer;
            if (timer > 10000)
            {
                error = true;
                printf("line hang\n");
                goto outside;
            }
        }
        tlen += len;
        if (g_lineBuf[0]==EQ_HSCAN_LINE_START)
        {
            row++;
            detectCodes(row, g_lineBuf+1, len-1);
            line_hLine(row, g_lineBuf+1, len-1);
        }
        else if (g_lineBuf[0]==EQ_VSCAN_LINE_START)
            line_vLine(row, vstate, g_lineBuf+1, len-1);

        if (eof || error)
            break;
    }
    
    outside:
    printf("Filling grid ended with error: %d and eof: %d \n", error, eof);

/* Filling grid end */

/* Sorting barcodes and calculating barcode stats */
    clusterCodes();
/* Sorting barcodes and calculating barcode stats end */

/* Marking barcode points on picture */
    fp_image = fopen(argv[1], "r+b");
    if (markBarcodesOnFile(fp_image))
    {
        fclose(fp_image);
        return -1;
    }
    fclose(fp_image);
/* Marking barcode points on picture end */

    printf("LINE GRID:\n");
    for (int i = 0; i < LINE_GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < LINE_GRID_WIDTH; ++j)
        {
            if (g_lineGrid[LINE_GRID_INDEX(j,i)])
            {
                printf("X", g_lineGrid[LINE_GRID_INDEX(j, i)]);
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }


    fp_image = fopen(argv[1], "r+b");
    if (NULL == fp_image)
    {
        fclose(fp_image);
        return -1;
    }


/* Marking line grid on picture */

    for (int y = 0; y < LINE_GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < LINE_GRID_WIDTH; ++x)
        {
            if (g_lineGrid[LINE_GRID_INDEX(x,y)] & EQ_NODE_FLAG_HLINE)
            {
                int pos = 16*LINE_GRID_WIDTH*y + 4*x;
                int fseek_ret = fseek(fp_image, 15 + 3*pos,0);
                int fwrite_ret = fwrite(maxx, 1, 1, fp_image);
            }

            if (y == 5)
            {
                int pos = 16*LINE_GRID_WIDTH*y + 4*x;
                int fseek_ret = fseek(fp_image, 15 + 3*pos + 2,0);
                int fwrite_ret = fwrite(maxx, 1, 1, fp_image);
            }
        }
    }

    int fseek_ret = fseek(fp_image, 0, 2);
    printf("fseek to end returned %d\n", fseek_ret);
    fclose(fp_image);
    
/* Marking line grid on picture end */

    if (g_b3Detected)
    {
        printf("\x1B[32mBarcode 3 detected!!!\n\x1B[0m");
    }
    if (g_b4Detected)
    {
        printf("\x1B[32mBarcode 4 detected!!!\n\x1B[0m");
    }

    return 0;
}

int loadEdgesFromFile(FILE *fp)
{
    if (NULL == fp)
    {
        printf("Couldn't load edges, FILE *fp == NULL!\n");
        return -1;
    }

    uint8_t line[CAM_RES3_WIDTH + 4];
    int line_counter = 0;
    int fread_size;

    while (fread_size = fread(line, 1, CAM_RES3_WIDTH - 4, fp))
    {
        /* Modulo 2 because it has different resolution in algorithm than here */
        if (line_counter % 2)
        {
            hScan(line);
        }
        ++line_counter;
    }

    edges[edges_write_index++] = EQ_FRAME_END;
    ++edges_produced;

    return 0;
}

int markEdgesOnFile(FILE *fp)
{
    if (NULL == fp)
    {
        printf("Couldn't mark edges, FILE *fp == NULL!\n");
        return -1;
    }

    int width = 316;
    int height = 208;
    uint8_t mark_value[1] = {0xf0};
    int hscan_line_counter = -1;

    for (int i = 0; i < edges_produced; ++i)
    {
        if (EQ_HSCAN_LINE_START == edges[i])
        {
            hscan_line_counter += 2;
            printf("EQ_HSCAN_LINE_START %d\n", hscan_line_counter);
        }
        else
        {
            if (edges[i] & EQ_NEGATIVE)
            {
                int pos = (edges[i] - EQ_NEGATIVE + 1)/2;
                printf("edge %4d:    -%d\n", i, pos);
                printf("fseek goes to %d\n", 15 + 3*hscan_line_counter*width + 3*pos + 2);
                int fseek_ret = fseek(fp, 15 + 3*hscan_line_counter*width + 3*pos + 2,0);
                printf("fseek returned %d\n", fseek_ret);
                int fwrite_ret = fwrite(mark_value, 1, 1, fp);
                if (fwrite_ret != 1)
                {
                    perror("fwriteerror: ");
                }
                printf("fwrite returned %d\n", fwrite_ret);
            }
            else
            {
                int pos = (edges[i] + 1)/2;
                printf("edge %4d:    %d\n", i, pos);
                printf("fseek goes to %d\n", 15 + 3*hscan_line_counter*width + 3*pos);
                int fseek_ret = fseek(fp, 15 + 3*hscan_line_counter*width + 3*pos,0);
                printf("fseek returned %d\n", fseek_ret);
                int fwrite_ret = fwrite(mark_value, 1, 1, fp);
                if (fwrite_ret != 1)
                {
                    perror("fwriteerror: ");
                }
                printf("fwrite returned %d\n", fwrite_ret);
            }
        }
    }

    printf("hscan_line_counter: %d\n", hscan_line_counter);
    int fseek_ret = fseek(fp, 0, 2);
    printf("fseek to end returned %d\n", fseek_ret);
    return 0;
}

int hScan(uint8_t *memy)
{
    int16_t i;
    int16_t end, diff;
    ENQUEUE_START();

//     if (eq_free()<CAM_RES3_WIDTH/2+CAM_RES3_HEIGHT)
//         return -1;
    ENQUEUE(EQ_HSCAN_LINE_START);

    i = -1;
    end = CAM_RES3_WIDTH - g_dist;

    // state 0, looking for either edge
loop0:
    i++;
    if (i>=end)
        goto loopex;
    diff = memy[i+g_dist]-memy[i];
    if (-g_thresh>=diff)
        goto edge0;
    if (diff>=g_thresh)
        goto edge1;
    goto loop0;

    // found neg edge
edge0:
        printf("Found negative edge for i: %d\n", i);
    ENQUEUE(i | EQ_NEGATIVE);
    i+=2;

    // state 1, looking for end of edge or pos edge
loop1:
    i++;
    if (i>=end)
        goto loopex;
    diff = memy[i+g_dist]-memy[i];
    if (-g_hThresh<diff)
        goto loop0;
    if (diff>=g_thresh)
        goto edge1;
    goto loop1;

    // found pos edge
edge1:
        printf("Found positive edge for i: %d\n", i);
    ENQUEUE(i);
    i+=2;

    // state 2, looking for end of edge or neg edge
loop2:
    i++;
    if (i>=end)
        goto loopex;
    diff = memy[i+g_dist]-memy[i];
    if (diff<g_hThresh)
        goto loop0;
    if (-g_thresh>=diff)    
        goto edge0;
    goto loop2;

loopex:
    ENQUEUE_END();
    return 0;

}


uint32_t readLine(uint16_t *mem, uint32_t size, bool *eof, bool *error)
{
    printf("Entering readLine\n");
    uint16_t len = edges_produced - edges_consumed;
    uint16_t i, j;
    uint8_t codes;

    for (i=0, j=edges_read_index, codes=0, *eof=false, *error=false; true; i++)
    {
        printf("readLine: i=%d, j=%d, codes=%d\n", i, j, codes);
        if (i>=len || i>=size)
        {
            printf("readLine returns 0, because i=%u, len=%u, size=%u\n", i, len, size);
            return 0;
        }

        mem[i] = edges[j];
        printf("mem[%d] = %d == edges[%d]\n", i, edges[j], j);

        if (edges[j]>=EQ_HSCAN_LINE_START)
        {
            printf("edges[%d] = %d >= EQ_HSCAN_LINE_START (%d)\n",j, edges[j], EQ_HSCAN_LINE_START);
            if (edges[j]==EQ_ERROR)
            {
                i++; // return and eat error code
                *error = true;
                break;
            }
            else if (edges[j]==EQ_FRAME_END)
            {
                i++; // return and eat eof code
                *eof = true;
                break;
            }
            codes++;
            if (codes>=2)
                // don't return code
                break;
        }
        j++;
        if (j==EQ_MEM_SIZE)
            j = 0;
    }

    // flush what we read
    edges_consumed += i;
    edges_read_index+= i;
    if (edges_read_index>=EQ_MEM_SIZE)
        edges_read_index -= EQ_MEM_SIZE;

    printf("readLine return %i\n", i);
    return i;
}


int line_hLine(uint8_t row, uint16_t *buf, uint32_t len)
{
    printf("Entering line_hLine\n");
    uint16_t j, index, bit0, bit1, col0, col1, lineWidth;

    for (j=0; buf[j]<EQ_HSCAN_LINE_START && buf[j+1]<EQ_HSCAN_LINE_START && j<len; j++)
    {
        bit0 = buf[j]&EQ_NEGATIVE;
        bit1 = buf[j+1]&EQ_NEGATIVE;
        col0 = buf[j]&~EQ_NEGATIVE;
        col1 = buf[j+1]&~EQ_NEGATIVE;

        printf("line_hLine, j=%d: bit0(%d), bit1(%d), col0(%d), col1(%d)\n", j, bit0, bit1, col0, col1);
        if (bit0!=0 && bit1==0)
        {
            printf("line_hLine: line found at (%d, %d) with width %d\n", ((col0 + col1)>>1), row << 1, col1 - col0);
            lineWidth = col1 - col0;
            if (g_minLineWidth<lineWidth && lineWidth<g_maxLineWidth)
            {
                index = LINE_GRID_INDEX((((col0+col1)>>1) + g_dist)>>3, row>>1);
                printf("line_hLine: found line is of good width, gets grid index %d\n", index);
                if (index<LINE_GRID_WIDTH*LINE_GRID_HEIGHT+8)
                    g_lineGrid[index] |= LINE_NODE_FLAG_HLINE;
                else
                    printf("\x1B[31mhigh index\n\x1B[0m");
            }
        }
    }
    return 0;
}

int line_vLine(uint8_t row, uint8_t *vstate, uint16_t *buf, uint32_t len)
{
    uint16_t i, index, bit0, col0, lineWidth;

    for (i=0; buf[i]<EQ_HSCAN_LINE_START && i<len; i++)
    {
        bit0 = buf[i]&EQ_NEGATIVE;
        col0 = (buf[i]&~EQ_NEGATIVE)>>2;
        if (bit0!=0) // neg
            vstate[col0] = row+1;
        else // bit0==0, pos
        {
            if (vstate[col0]!=0)
            {
                lineWidth = (row - (vstate[col0]-1))<<2; // multiply by 4 because vertical is subsampled by 4
                if (g_minLineWidth<lineWidth && lineWidth<g_maxLineWidth && col0<LINE_VSIZE)
                {
                    index = LINE_GRID_INDEX(col0>>1, (row - (lineWidth>>3))>>1);
                    if (index<LINE_GRID_WIDTH*LINE_GRID_HEIGHT+8)
                        g_lineGrid[index] |= LINE_NODE_FLAG_VLINE;
                    else
                        printf("high index\n");
                }
                vstate[col0] = 0;
            }
        }
    }
    return 0;
}

void detectCodes(uint8_t row, uint16_t *buf, uint32_t len)
{
    printf("Entering detectCodes\n");
    uint16_t j, x, y, bit0, bit1, col0, col1, barcodeLineWidth;

    for (j=0; buf[j]<EQ_HSCAN_LINE_START && buf[j+1]<EQ_HSCAN_LINE_START && j<len; j++)
    {
        bit0 = buf[j]&EQ_NEGATIVE;
        bit1 = buf[j+1]&EQ_NEGATIVE;
        col0 = buf[j]&~EQ_NEGATIVE;
        col1 = buf[j+1]&~EQ_NEGATIVE;

        if (bit0!=0 && bit1==0)
        {
            x = ((col0 + col1) >> 2);
            y = (row << 1) + 1;
            barcodeLineWidth = col1 - col0;
            printf("detectCodes: BarcodeLine found at (%d, %d) with width %d\n", x, y, barcodeLineWidth);
            if (g_minBarcodeLineWidth<barcodeLineWidth && barcodeLineWidth<g_maxBarcodeLineWidth)
            {
                printf("detectCodes: found BarcodeLine is of good width at (avgx, y) = (%d, %d)\n", x, y);
                for (int i = 0; i < BARCODELINES_SIZE; ++i)
                {
                    BarcodeLine &b = g_barcodeLines[i];
                    if (0 == b.pointsIndex)
                    {
                        b.points[b.pointsIndex++] = {x, y};
                        b.x = x;
                        b.y = y;
                        b.width = barcodeLineWidth;
                        g_blLastValid = i;
                        i = BARCODELINES_SIZE; 
                    }
                    else
                    {
                        if (std::abs(b.points[b.pointsIndex - 1].x - x) <= g_maxBarcodeLineJitter && std::abs(b.points[b.pointsIndex - 1].y - y) <= g_maxBarcodeLineJitter)
                        {
                            if (b.pointsIndex < BARCODELINE_POINTS_SIZE)
                            {
                                b.points[b.pointsIndex++] = {x, y};
                                b.x += x;
                                b.y += y;
                                b.width += barcodeLineWidth;
                            }
                            else
                            {
                                b.points[BARCODELINE_POINTS_SIZE - 1] = {x, y};
                            }
                            i = BARCODELINES_SIZE;
                        }
                    }
                }
            }
        }

    }
}

int markBarcodesOnFile(FILE *fp)
{
    if (NULL == fp)
    {
        printf("Couldn't mark barcodes, FILE *fp == NULL!\n");
        return -1;
    }

    uint8_t markValues[2] = {0xf0, 0xf0};

    for (int i = 0; i <= g_blLastValid; ++i)
    {
        const BarcodeLine &b = g_barcodeLines[i];
        printf("BarcodeLines[%d]:\n", i);
        for (int point_counter = 0; point_counter < b.pointsIndex; ++point_counter)
        {
            uint32_t pos = b.points[point_counter].y*CAM_RES2_WIDTH + b.points[point_counter].x;
            printf("BarcodeLine[%d] (%d, %d), pos = %d\n", i, b.points[point_counter].x, b.points[point_counter].y, pos);
            int fseek_ret = fseek(fp, 15 + 3*pos,0);
            int fwrite_ret = fwrite(markValues, 1, 2, fp);
        }
    }

    int fseek_ret = fseek(fp, 0, 2);
    return 0;
}

void clusterCodes()
{
    for (int i = 0; i <= g_blLastValid; i++)
    {
        BarcodeLine &b = g_barcodeLines[i];

        if (1 == b.pointsIndex)
        {
            std::swap(g_barcodeLines[i], g_barcodeLines[g_blLastValid--]);
            --i;
        }
        else
        {
            b.x /= b.pointsIndex;
            b.y /= b.pointsIndex;
            b.width /= b.pointsIndex;
            int16_t dx = b.points[b.pointsIndex - 1].x - b.points[0].x;
            int16_t dy = (b.points[b.pointsIndex - 1].y - b.points[0].y);
            b.a = (1000 * dx) / dy;
            b.b = 1000*b.y - b.x*b.a;
        }
    }

    std::sort(&g_barcodeLines[0], &g_barcodeLines[g_blLastValid], [&](BarcodeLine b1, BarcodeLine b2) { return b1.points[0].x < b2.points[0].x; });

    for (int i = 0; i <= g_blLastValid; ++i)
    {
        printf("barcodelines[%d]: x=%d, y=%d, a=%d, b=%d, width=%d\n", i, g_barcodeLines[i].x, g_barcodeLines[i].y, g_barcodeLines[i].a, g_barcodeLines[i].b, g_barcodeLines[i].width);
    }
    
    for (int i = 0; i < g_blLastValid; ++i)
    {
        uint16_t y = (g_barcodeLines[i].y + g_barcodeLines[i+1].y) >> 1;
        uint16_t distance = g_barcodeLines[i+1].x - g_barcodeLines[i].x;
        if (g_bcIndex)
        {
            bool newCandidate = true;
            for (BarcodeCandidate *iter = &g_barcodeCandidates[0]; iter < &g_barcodeCandidates[g_bcIndex]; ++iter)
            {
                if ( std::abs(iter->y - y) < 10 && std::abs(iter->distance - distance) < 10)
                {
                    iter->y = (iter->y * iter->count + y) / (iter->count + 1);
                    iter->distance = (iter->distance * iter->count + distance) / (iter->count + 1);
                    iter->count++;
                    newCandidate = false;
                    break;
                }
            }
            if (newCandidate)
            {
                g_barcodeCandidates[g_bcIndex++] = {y, distance, 2};
            }
        }
        else
        {
            g_barcodeCandidates[g_bcIndex++] = {y, distance, 2};
        }
    }

    for (int i = 0; i < g_bcIndex; ++i)
    {
        if (g_barcodeCandidates[i].count == 3) g_b3Detected = true;
        if (g_barcodeCandidates[i].count == 4) g_b4Detected = true;
        printf("barcodeCandidates[%d]: y=%d, distance=%d, count=%d\n", i, g_barcodeCandidates[i].y, g_barcodeCandidates[i].distance, g_barcodeCandidates[i].count);
    }
}

