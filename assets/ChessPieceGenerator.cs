using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

class ChessPieceImageGenerator
{
    static void Main()
    {
        string assetsDir = @"C:\Users\mouaz\source\repos\Schack\Schack\assets";
        if (!System.IO.Directory.Exists(assetsDir))
        {
            System.IO.Directory.CreateDirectory(assetsDir);
        }

        int size = 80;
        
        Console.WriteLine("Generating professional chess piece images...");
        Console.WriteLine();

        // White pieces
        GenerateKing(assetsDir, "white_king.png", Color.White, Color.Black, size);
        GenerateQueen(assetsDir, "white_queen.png", Color.White, Color.Black, size);
        GenerateRook(assetsDir, "white_rook.png", Color.White, Color.Black, size);
        GenerateBishop(assetsDir, "white_bishop.png", Color.White, Color.Black, size);
        GenerateKnight(assetsDir, "white_knight.png", Color.White, Color.Black, size);
        GeneratePawn(assetsDir, "white_pawn.png", Color.White, Color.Black, size);

        // Black pieces
        GenerateKing(assetsDir, "black_king.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);
        GenerateQueen(assetsDir, "black_queen.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);
        GenerateRook(assetsDir, "black_rook.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);
        GenerateBishop(assetsDir, "black_bishop.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);
        GenerateKnight(assetsDir, "black_knight.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);
        GeneratePawn(assetsDir, "black_pawn.png", Color.FromArgb(40, 40, 40), Color.FromArgb(220, 220, 220), size);

        Console.WriteLine();
        Console.WriteLine("All chess piece images generated successfully!");
    }

    static void GeneratePawn(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // Head
            g.FillEllipse(new SolidBrush(fill), centerX - 12, centerY - 20, 24, 24);
            g.DrawEllipse(new Pen(outline, 2), centerX - 12, centerY - 20, 24, 24);

            // Body
            g.FillRectangle(new SolidBrush(fill), centerX - 8, centerY - 5, 16, 15);
            g.DrawRectangle(new Pen(outline, 2), centerX - 8, centerY - 5, 16, 15);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRook(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // Base
            g.FillRectangle(new SolidBrush(fill), centerX - 15, centerY - 15, 30, 35);
            g.DrawRectangle(new Pen(outline, 2), centerX - 15, centerY - 15, 30, 35);

            // Battlements
            for (int i = 0; i < 3; i++)
            {
                g.FillRectangle(new SolidBrush(fill), centerX - 13 + i * 10, centerY - 25, 8, 10);
                g.DrawRectangle(new Pen(outline, 1), centerX - 13 + i * 10, centerY - 25, 8, 10);
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateKnight(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // L-shape
            Point[] points = new Point[]
            {
                new Point(centerX - 10, centerY + 10),
                new Point(centerX - 5, centerY - 10),
                new Point(centerX + 5, centerY - 15),
                new Point(centerX + 10, centerY - 10),
                new Point(centerX + 10, centerY + 10),
                new Point(centerX + 2, centerY + 15),
                new Point(centerX - 10, centerY + 15)
            };

            g.FillPolygon(new SolidBrush(fill), points);
            g.DrawPolygon(new Pen(outline, 2), points);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateBishop(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // Top circle
            g.FillEllipse(new SolidBrush(fill), centerX - 5, centerY - 28, 10, 10);
            g.DrawEllipse(new Pen(outline, 2), centerX - 5, centerY - 28, 10, 10);

            // Triangle body
            Point[] points = new Point[]
            {
                new Point(centerX, centerY - 15),
                new Point(centerX - 15, centerY + 20),
                new Point(centerX + 15, centerY + 20)
            };

            g.FillPolygon(new SolidBrush(fill), points);
            g.DrawPolygon(new Pen(outline, 2), points);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateQueen(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // Base circle
            g.FillEllipse(new SolidBrush(fill), centerX - 18, centerY - 15, 36, 36);
            g.DrawEllipse(new Pen(outline, 2), centerX - 18, centerY - 15, 36, 36);

            // Crown points
            for (int i = 0; i < 5; i++)
            {
                double angle = i * 72.0 - 90.0;
                float radius = 22.0f;
                float px = centerX + (float)(radius * Math.Cos(angle * Math.PI / 180.0));
                float py = centerY - 15 + (float)(radius * Math.Sin(angle * Math.PI / 180.0));

                g.FillEllipse(new SolidBrush(outline), px - 4, py - 4, 8, 8);
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateKing(string dir, string filename, Color fill, Color outline, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            // Base circle
            g.FillEllipse(new SolidBrush(fill), centerX - 16, centerY - 12, 32, 32);
            g.DrawEllipse(new Pen(outline, 2), centerX - 16, centerY - 12, 32, 32);

            // Cross
            g.FillRectangle(new SolidBrush(outline), centerX - 2, centerY - 32, 4, 18);
            g.FillRectangle(new SolidBrush(outline), centerX - 7, centerY - 25, 14, 4);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }
}
