using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LPC_PDF_PARSER;
using System.Text.RegularExpressions;

namespace LPC_PDF_PARSER
{
    static class Extensions
    {
        public static bool IsUpper(this string value)
        {
            // Consider string to be uppercase if it has no lowercase letters.
            for (int i = 0; i < value.Length; i++)
            {
                if (char.IsLower(value[i]))
                {
                    return false;
                }
            }

            for(int i = 0; i < value.Length; i++)
            {
                if (char.IsLetter(value[i]))
                {
                    return true;
                }
            }
            return false;
        }

        public static bool IsBitNumber(this string value)
        {

            if (value.IndexOf(':') >= 0)
            {
                string[] values = value.Split(':');

                if (values[0].IsInt() && values[1].IsInt())
                {
                    return true;
                }
            } 
            else
            {
                if(value.IsInt())
                {
                    return true;
                }
            }

            return false;
        }

        public static bool IsInt(this string value)
        {
            for (int i = 0; i < value.Length; i++)
            {
                if (!char.IsNumber(value[i]))
                {
                    return false;
                }
            }
            return true;
        }

        public static bool IsLower(this string value)
        {
            // Consider string to be lowercase if it has no uppercase letters.
            for (int i = 0; i < value.Length; i++)
            {
                if (char.IsUpper(value[i]))
                {
                    return false;
                }
            }
            return true;
        }
    }


    class Register
    {
        public string name;
        public string description;
        string adress;
        List<BitDescription> bitDescriptions;

        public Register()
        {
            bitDescriptions = new List<BitDescription>();
        }



        bool hasSymbolText(string line)
        {
            string [] items = line.Split(' ');
            int symbolIndex = 1;
            if (items.Length > 2)
            {
                if (items[0].Length == 0)
                {
                    symbolIndex++;
                }
            }
            else
            {
                return false;
            }

            if (items[symbolIndex - 1].IndexOf('x') >= 0)
            {
                return false;
            }

            if (items[symbolIndex].IsUpper())
            {
                return true;
            }

            return false;
        }

        int findStartIndex(string data, int index)
        {

            string dataTemp = data.Substring(0, index).Replace("\r\n", " ");
            string[] datas = dataTemp.Split(' ');
            for (int i = datas.Length - 3; i >= 1; i--)
            {
                if (datas[i].IsUpper() && datas[i-1].IsBitNumber())
                {
                    return data.IndexOf(datas[i - 1] + " " + datas[i]);
                }
            }
            return -1;
        }

        int findFirst(string data, string [] datas)
        {
            foreach (string d in datas)
            {
                int index = data.IndexOf(d);
                if (index >= 0)
                {
                    return index;
                }
            }
            return -1;
        }

        string symbolWithoutBreakLine(string data)
        {
            return data.Replace("_\r\n", "_");
        }

        string [] splitRegistersUnits(string data)
        {
            data = symbolWithoutBreakLine(data);

            string dataTemp = data;
            List<int> startIndexes = new List<int>();

            List<string> datas = new List<string>();
            int moveIndex = 0;
            while(true)
            {
                int firstRW = dataTemp.IndexOf("R/W\r\n");
                int firstR = dataTemp.IndexOf("R\r\n");
                int firstW = dataTemp.IndexOf("W\r\n");

                if (firstRW == -1 && firstR == -1 && firstW == -1)
                {
                    firstRW = dataTemp.IndexOf("R/W");
                    firstR = dataTemp.IndexOf("\r\nR");
                    //firstW = dataTemp.IndexOf(" W");
                    if (firstRW == -1 && firstR == -1 && firstW == -1)
                    {
                        break;
                    } 
                }
                
                List<int> values = new List<int>();
                if (firstRW >= 0)
                {
                    values.Add(firstRW);
                }

                if (firstR >= 0)
                {
                    values.Add(firstR);
                }

                if (firstW >= 0)
                {
                    values.Add(firstW);
                }

                int endIndex = values.Min() + 3;
                int start = findStartIndex(dataTemp, endIndex);

                if (start != -1)
                {
                    datas.Add(dataTemp.Substring(start, endIndex - start));
                }
                
                dataTemp = dataTemp.Substring(endIndex);
            }
            
            foreach(string ss in datas)
            {
                startIndexes.Add(data.IndexOf(ss));
            }

            string[] registerFields = new string[startIndexes.Count];
            if (startIndexes.Count >= 3)
            {
                registerFields[0] = data.Substring(startIndexes[0], startIndexes[1] - startIndexes[0]);

                for (int i = 1; i < startIndexes.Count - 1; i++)
                {
                    registerFields[i] = data.Substring(startIndexes[i] , startIndexes[i + 1] - startIndexes[i]);
                }

                registerFields[startIndexes.Count - 1] = data.Substring(startIndexes[startIndexes.Count - 1]);
            }
            else if (startIndexes.Count == 1)
            {
                registerFields[startIndexes.Count - 1] = data.Substring(startIndexes[startIndexes.Count - 1]);
            }
            else if (startIndexes.Count == 2)
            {
                registerFields[0] = data.Substring(startIndexes[0], startIndexes[1] - startIndexes[0]);
                registerFields[startIndexes.Count - 1] = data.Substring(startIndexes[startIndexes.Count - 1]);
            }
            return registerFields;
        }

        public string parse(string data_)
        {
            string [] registerFields = splitRegistersUnits(data_);

            string data = data_.Replace('\n', ' ');

            string[] lines = data.Split('\r');

            string tableTitle = "";
            bool startRead = false;
            bool detedtedTitleTable = false;

            foreach(string line in lines)
            {
                if (line.IndexOf("Table") >= 0 && line.IndexOf("bit description") >= 0)
                {
                    tableTitle = line;
                    detedtedTitleTable = true;
                    break;
                }
                else if (line.IndexOf("Table") >= 0)
                {
                    startRead = true;
                    tableTitle = line;
                }
                else if (line.IndexOf("description") >= 0)
                {
                    startRead = false;
                    tableTitle += " " + line;
                    detedtedTitleTable = true;
                    break;
                }
                else
                {
                    if (startRead)
                    {
                        tableTitle += " " + line;
                    }
                }
            }

            if(detedtedTitleTable)
            {
                int nameIndex = tableTitle.IndexOf(" (") + 2;
                int addresIndex = tableTitle.IndexOf("address");

                name = tableTitle.Substring(nameIndex, addresIndex - 2 - nameIndex);
                addresIndex += ("address").Length + 1;

                adress = tableTitle.Substring(addresIndex, tableTitle.IndexOf(") ") - addresIndex).Replace(" ", "");
            } 
            
            foreach (string ss in registerFields)
            {
                bitDescriptions.Add(new BitDescription(ss));
            }

            for (int i =0; i < bitDescriptions.Count; i++)
            {
                if (!bitDescriptions[i].parse())
                {
                    bitDescriptions.RemoveAt(i);
                    i--;
                }
            }
            
            return data;
        }

        public string export(string segmentName)
        {
            string exported = "/* " + name + " " + description + " at " + adress + "*/\r\n";
            foreach(BitDescription bit in bitDescriptions)
            {
                exported += bit.export(segmentName, name);
            }


            return exported;
        }

    }
}
