using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LPC_PDF_PARSER
{


    class Value
    {
        byte[] stringToByteArray(string hex)
        {
            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        public UInt32 value;
        public string description;
        public Value(byte value, string description)
        {
            this.value = value;
            this.description = description;
        }

        public Value(string value, string description)
        {
            if (value.Length == 1)
            {
                this.value = Convert.ToUInt32(value);
            } 
            else if (value.IndexOf('x') >= 0)
            {
                value = value.Remove(0, value.IndexOf('x') + 1).Replace(" ", "");
                this.value = uint.Parse(value, System.Globalization.NumberStyles.HexNumber);
            }
            // 
            this.description = description;
        }
    }

    class BitDescription
    {
        public byte startBit;
        public byte stopBit;
        public string symbol;
        string description;
        string resetValue;
        string access;

        List<Value> values;
        List<string> datas;
        string data;

        static int IntPow(int x, int pow)
        {
            int ret = 1;
            while (pow != 0)
            {
                if ((pow & 1) == 1)
                    ret *= x;
                x *= x;
                pow >>= 1;
            }
            return ret;
        }

        public BitDescription(string data)
        {
            this.data = data;
            startBit = 255;
            stopBit = 255;
            values = new List<Value>();
            datas = new List<string>();
        }

        void addValue(byte value, string description)
        {
            values.Add(new Value(value, description));
        }

        public void addData(string data)
        {
            datas.Add(data);
        }

        bool hasAccessData(string line)
        {
            if (line.IndexOf("R/W") >= 0)
            {
                return true;
            }

            return false;
        }

        void mergeDescriptionToOneLine()
        {
            if (hasAccessData(datas[0]))
            {

            }
            else
            {
                bool hasAccessDataFlag = false;
                int i;
                for(i = 0; i < datas.Count; i++)
                {
                    if (hasAccessData(datas[i]))
                    {
                        hasAccessDataFlag = true;
                        break;
                    }
                }

                if (hasAccessDataFlag)
                {
                    for (int j = 0; j < i; j++)
                    {
                        datas[0] = datas[0] + " " + datas[1];
                        datas.RemoveAt(1);
                    }
                }

            }

        }

        void removeFirstBlankSpaces()
        {
            for (int i = 0; i < datas.Count; i++)
            {
                if (datas[i][0] == ' ')
                {
                    datas[i] = datas[i].Remove(0, 1);
                }
            }
        }

        bool getBits()
        {
            string [] data = datas[0].Split(' ');

            if(data[0].IndexOf(':') >= 0)
            {
                string [] bits = data[0].Split(':');
                startBit = Convert.ToByte(bits[1]);
                stopBit = Convert.ToByte(bits[0]);
                datas[0] = datas[0].Remove(0, data[0].Length + 1);
                return true;
            }
            else
            {
                if (data[0].IsInt())
                {
                    startBit = Convert.ToByte(data[0]);
                    stopBit = startBit;
                    datas[0] = datas[0].Remove(0, data[0].Length + 1);
                    return true;
                }
            }

            return false;
        }

        bool isReservedValue(string value)
        {
            if (value.IndexOf("Reserved") >=0 )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool OnlyHexInString(string test)
        {
            // For C-style hex notation (0xFF) you can use @"\A\b(0[xX])?[0-9a-fA-F]+\b\Z"
            return System.Text.RegularExpressions.Regex.IsMatch(test, @"\A\b(0[xX])?[0-9a-fA-F]+\b\Z");
        }


        void parseValues()
        {
            if (datas.Count >= 2)
            {
                for(int i = 1; i < datas.Count; i++)
                {
                    string line = datas[i];

                    if (isReservedValue(line))
                    {
                        int space = line.IndexOf(' ');

                        values.Add(new Value(line.Remove(space), "Reserved"));
                        continue;
                    }

                    if (OnlyHexInString(line.Split(' ')[0]))
                    {
                        int spaceIndex = line.IndexOf(' ');
                        if (spaceIndex > 0)
                        {
                            values.Add(new Value(line.Remove(spaceIndex), line.Remove(0, spaceIndex + 1)));
                        }
                    }
                    else
                    {
                        if (values.Count >= 1)
                        {
                            values[values.Count - 1].description += " " + line;
                            datas.RemoveAt(i);
                        }

                    }
                
                }
            }
        }

        void parseHeader()
        {
            string[] data = datas[0].Split(' ');
            if (data[0].IsUpper())
            {
                symbol = data[0];
            }

            access = data[data.Length - 1];
            resetValue = data[data.Length - 2];

            for(int i =1; i < data.Length - 2; i++)
            {
                description += data[i] + " ";
            }
        }

        public bool parse()
        {
            string[] temp = data.Replace("\r", "").Split('\n');
            foreach(string t in temp)
            {
                if (t != "")
                {
                    datas.Add(t);
                }
            }

            mergeDescriptionToOneLine();
            removeFirstBlankSpaces();
            if (getBits())
            {
                parseValues();
                parseHeader();
            }
            else
            {
                return false;
            }

            return true;
        }

        public string export(string segmentName, string registerName)
        {
            string exported = "";
            int posiibleValuesCounter = IntPow(2, stopBit - startBit + 1);
            string name = segmentName + "_" + registerName + "_" + symbol;

            exported += "/* " + name + " " + description + "\r\n";
            exported += " * default: " + resetValue + "\r\n";
            foreach (Value val in values)
            {
                if (val.description != "Reserved")
                {
                    exported += " * \t 0x" + val.value.ToString("X2") + " " + val.description + "\r\n";
                }
                
            }
            exported += " */\r\n";
            exported += "#define " + name + "_SHIFT\t(" + startBit.ToString() + "U)\r\n";
            exported += "#define " + name + "_MASK" + " \t(0x" + (posiibleValuesCounter - 1).ToString("X2") + "U << " + name + "_SHIFT)\r\n";
            exported += "#define " + name + "(x)\t\t(((uint32_t)(((uint32_t)(x)) << " + name + "_SHIFT" + ")) & " + name + "_MASK" + ")\r\n";

            exported += "\r\n";
            return exported;
        }
        
    }
}
