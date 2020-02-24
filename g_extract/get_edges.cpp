#include <stdint.h>
#include <stdio.h>

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
#define EQ_STREAM_LINES	    0x0002

#define EQ_NODE_SIZE        12

#define EQ_NODE_FLAG_HLINE  0x0400
#define EQ_NODE_FLAG_VLINE  0x0800

uint16_t edges[EQ_MEM_SIZE];
uint16_t edges_index = 0;
uint16_t edges_produced = 0;
uint16_t edges_consumed = 0;

uint16_t g_dist = 4;
uint16_t g_thresh = 20;
uint16_t g_hThresh = 20*3/5;

#define ENQUEUE_START() \
	uint16_t *data = edges; \
	uint16_t writeIndex = edges_index; \
	uint16_t produced = 0 

#define ENQUEUE(val) \
	data[writeIndex++] = val; \
	produced++; \
	if (writeIndex==EQ_MEM_SIZE) \
		writeIndex = 0 

#define ENQUEUE_END() \
	edges_index = writeIndex; \
	edges_produced += produced 

int hScan(uint8_t *memy);

uint32_t readLine(uint16_t *mem, uint32_t size, bool *eof, bool *error);


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("No filename given!\n");
        return -1;
    }


    FILE *fp = fopen("green_bytes_only", "rb");
    if (NULL == fp)
    {
        printf("Couldn't open file!\n");
        return -1;
    }

    uint8_t line[CAM_RES3_WIDTH + 4];

    int line_counter = 0;
    int fread_size;

    while (fread_size = fread(line, 1, CAM_RES3_WIDTH - 4, fp))
    {
        printf("fread_size: %d\n", fread_size);
        hScan(line);
        ++line_counter;
    }


    printf("edges:\n");
    for (int i = 0; i < EQ_MEM_SIZE; ++i)
    {
        printf("%d, ", edges[i]);
    }
    printf("\n");



    fclose(fp);

    int width = 316;
    int height = 208;
    FILE *f_edges = fopen(argv[1], "r+b");

    if (NULL == f_edges)
    {
        printf("Couldn't open f_edges\n");
        fclose(f_edges);
        return -1;
    }

    uint8_t maxx[1] = {0xf0};
    int hscan_line_counter = -1;
    for (int i = 0; i < edges_produced; ++i)
    {
        if (EQ_HSCAN_LINE_START == edges[i])
        {
            ++hscan_line_counter;
            printf("EQ_HSCAN_LINE_START %d\n", hscan_line_counter);
        }
        else
        {
            if (edges[i] & EQ_NEGATIVE)
            {
                int pos = (edges[i] - EQ_NEGATIVE + 1)/2;
                printf("edge %4d:    -%d\n", i, pos);
                printf("fseek goes to %d\n", 15 + 3*hscan_line_counter*width + 3*pos + 2);
                int fseek_ret = fseek(f_edges, 15 + 3*hscan_line_counter*width + 3*pos + 2,0);
                printf("fseek returned %d\n", fseek_ret);
                int fwrite_ret = fwrite(maxx, 1, 1, f_edges);
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
                int fseek_ret = fseek(f_edges, 15 + 3*hscan_line_counter*width + 3*pos,0);
                printf("fseek returned %d\n", fseek_ret);
                int fwrite_ret = fwrite(maxx, 1, 1, f_edges);
                if (fwrite_ret != 1)
                {
                    perror("fwriteerror: ");
                }
                printf("fwrite returned %d\n", fwrite_ret);
            }
        }
    }


    printf("line_counter: %d\nhscan_line_counter: %d\n", line_counter, hscan_line_counter);
    int fseek_ret = fseek(f_edges, 0, 2);
    printf("fseek to end returned %d\n", fseek_ret);
    fclose(f_edges);



    

    return 0;
}

int hScan(uint8_t *memy)
{
	int16_t i;
	int16_t end, diff;
	ENQUEUE_START();
	
// 	if (eq_free()<CAM_RES3_WIDTH/2+CAM_RES3_HEIGHT)
// 		return -1;
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
	uint16_t len = edges_produced - edges_consumed;
	uint16_t i, j;
	uint8_t codes;
	
	for (i=0, j=edges_index, codes=0, *eof=false, *error=false; true; i++)
	{
		if (i>=len || i>=size)
			return 0;
		
		mem[i] = edges[j];
		
		if (edges[j]>=EQ_HSCAN_LINE_START)
		{
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
	edges_index += i;
	if (edges_index>=EQ_MEM_SIZE)	
		edges_index -= EQ_MEM_SIZE;

	return i;
}


