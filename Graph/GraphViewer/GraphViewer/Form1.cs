using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics;

namespace GraphViewer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            chart1.MouseWheel += new System.Windows.Forms.MouseEventHandler(chart1_MouseWheel);
            openFileDialog1.ShowDialog();
        }

        private void chart1_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            // Update the drawing based upon the mouse wheel scrolling.

            int numberOfTextLinesToMove = e.Delta * SystemInformation.MouseWheelScrollLines/120;
            Point position = PointToClient(MousePosition);
            if ((position.X > (this.chart1.Location.X)) && (position.X < (this.chart1.Location.X + this.chart1.Size.Width)) && (position.Y > (this.chart1.Location.Y)) && (position.Y < (this.chart1.Location.Y + this.chart1.Size.Height)))
            {
                position.X -= this.chart1.Location.X;
                position.Y -= this.chart1.Location.Y;
                Debug.WriteLine(Convert.ToString("x: " + position.X + " y: " + position.Y));
            }
            

        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            this.chart1.Height = this.Size.Height - 63;
            this.chart1.Width = this.Size.Width - 40;
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            System.IO.StreamReader file = new System.IO.StreamReader(openFileDialog1.FileName);
            string line;
            line = file.ReadLine();
            string[] headers = line.Split(new Char[] { '\t' });
            int i = 0;
            foreach(string header in headers)
            {
                this.chart1.Series.Add(header);
                this.chart1.Series[i].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
                if(header == "Relay")
                {
                    this.chart1.Series[i].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Area;
                    this.chart1.Series[i].Color = Color.FromArgb(60, 0, 100, 25);
                }
                i++;
            }
            Debug.WriteLine("Добавили заголовки");
            while ((line = file.ReadLine()) != null) //читаем данные из файла, пока не встретится пустая строка
            {
                string[] values = line.Split(new Char[] { '\t' });
                i = 0;
                foreach(string value in values)
                {
                    try
                    {
                        this.chart1.Series[i].Points.AddY(Int64.Parse(value));
                    }
                    catch (FormatException a)
                    {
                        Debug.WriteLine(a.Message);
                    }
                    i++;
                }
                Debug.WriteLine("Добавляем точки");
            }
            file.Close(); // закрываем файл
            double max = this.chart1.Series["Original"].Points.FindMaxByValue().YValues[0];
            if (this.chart1.Series["SMA"].Points.FindMaxByValue().YValues[0]>max)
            {
                max = this.chart1.Series["SMA"].Points.FindMaxByValue().YValues[0];
            }
            double min = this.chart1.Series["Original"].Points.FindMinByValue().YValues[0];
            if (this.chart1.Series["SMA"].Points.FindMinByValue().YValues[0] > min)
            {
                min = this.chart1.Series["SMA"].Points.FindMinByValue().YValues[0];
            }
            Debug.WriteLine(max);
            for (int j = 0; j < this.chart1.Series["Relay"].Points.Count; j++ )
            {
                this.chart1.Series["Relay"].Points[j].YValues[0]*= max/8 + (max/8)*0.1;
                Debug.WriteLine(this.chart1.Series["Relay"].Points[j].YValues[0]);
            }
            //this.chart1.ChartAreas[0].AxisY.Maximum = max + max * 0.05;
            //this.chart1.ChartAreas[0].AxisY.Minimum = min - min * 0.05;
            this.chart1.Series.RemoveAt(2);
            Debug.WriteLine("Точки обработаны");
        }
    }
}
