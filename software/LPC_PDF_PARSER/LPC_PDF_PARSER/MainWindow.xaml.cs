using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LPC_PDF_PARSER;

namespace LPC_PDF_PARSER
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        List<BitDescription> bitDescriptions;

        Register register;

        string segmentName = "CCU";

        public MainWindow()
        {
            InitializeComponent();
            // bitDescriptions = new List<BitDescription>();

            register = new Register();
        }

        private void PARSE_Button_Click(object sender, RoutedEventArgs e)
        {
            register.parse(Clipboard.GetText());
            string exported = register.export(segmentName);
            Clipboard.SetText(exported);
            RESULT_BOX.Text = exported;
            RegisterNameText.Text = register.name;
            // RegisterDescriptionText = 
        }

        private void ReExport_Click(object sender, RoutedEventArgs e)
        {
            register.name = RegisterNameText.Text;
            string exported = register.export(segmentName);
            Clipboard.SetText(exported);
            RESULT_BOX.Text = exported;
        }
    }
}
