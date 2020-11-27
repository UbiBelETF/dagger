using System;
using System.Drawing;
using System.Drawing.Text;
using System.IO;

namespace TextKnife
{
    public class Options
    {
        public string Filename { get; set; }

        public string Name { get; set; }

        public int Size { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("Usage: TextKnife <name w/ no extension> <TTF filename> <font size>");
                return;
            }

            Options options = new Options
            {
                Name = args[0],
                Filename = args[1],
                Size = int.Parse(args[2])
            };


            if (!File.Exists(options.Filename))
            {
                Console.WriteLine($"Error: Font file {options.Filename} not found.");
                return;
            }

            if (options.Name == null || options.Name.Trim().Length == 0)
            {
                Console.WriteLine($"Error: Name cannot be empty.");
                return;
            }

            if (options.Size <= 5 || options.Size > 128)
            {
                Console.WriteLine($"Error: Font size has to be between 5 and 128.");
                return;
            }

            CreateBitmapFont(options);
        }

        static void CreateBitmapFont(Options options)
        {
            var collection = new PrivateFontCollection();
            collection.AddFontFile(options.Filename);

            var font = new Font(collection.Families[0], options.Size, FontStyle.Regular);
            var brush = new SolidBrush(Color.White);

            SizeF largest = new SizeF(0, 0);

            {
                using var bmp = new Bitmap(1, 1);
                using var graphics = Graphics.FromImage(bmp);
                for (int i = 32; i < 128; i++)
                {
                    var size = graphics.MeasureString($"{(char)i}", font);
                    largest.Width += size.Width;
                    var h = Math.Ceiling(size.Height);                    
                    if (largest.Height < h)
                        largest.Height = (int)h;
                }
            }

            {
                using var spritesheet = new StreamWriter($"{options.Name}.spritesheet");
                using var bmp = new Bitmap((int)largest.Width + 1, (int)largest.Height + 1);
                using var graphics = Graphics.FromImage(bmp);

                graphics.Clear(Color.Transparent);

                int x = 0;
                for (int i = 32; i < 128; i++)
                {
                    var input = $"{(char)i}";
                    var size = graphics.MeasureString(input, font);
                    var w = (int)Math.Ceiling(size.Width);
                    var h = (int)Math.Ceiling(size.Height);

                    graphics.DrawString(input, font, brush, x, 0);
                    spritesheet.WriteLine($"{i} {x} 0 {w} {h}");
                    x += (int)Math.Ceiling(size.Width);
                }

                bmp.Save($"{options.Name}.png");
            }
        }
    }
}
