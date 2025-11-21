using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

class EnhancedChessPieceGenerator
{
    static void Main()
    {
        string assetsDir = @"C:\Users\mouaz\source\repos\Schack\Schack\assets";
        if (!System.IO.Directory.Exists(assetsDir))
        {
            System.IO.Directory.CreateDirectory(assetsDir);
        }

        int size = 80;
        
        Console.WriteLine("Generating professional 3D-style chess piece images...");
        Console.WriteLine();

        // White pieces - realistic style with gradient and shading
        GenerateRealisticKing(assetsDir, "white_king.png", true, size);
        GenerateRealisticQueen(assetsDir, "white_queen.png", true, size);
        GenerateRealisticRook(assetsDir, "white_rook.png", true, size);
        GenerateRealisticBishop(assetsDir, "white_bishop.png", true, size);
        GenerateRealisticKnight(assetsDir, "white_knight.png", true, size);
        GenerateRealisticPawn(assetsDir, "white_pawn.png", true, size);

        // Black pieces - realistic style with gradient and shading
        GenerateRealisticKing(assetsDir, "black_king.png", false, size);
        GenerateRealisticQueen(assetsDir, "black_queen.png", false, size);
        GenerateRealisticRook(assetsDir, "black_rook.png", false, size);
        GenerateRealisticBishop(assetsDir, "black_bishop.png", false, size);
        GenerateRealisticKnight(assetsDir, "black_knight.png", false, size);
        GenerateRealisticPawn(assetsDir, "black_pawn.png", false, size);

        Console.WriteLine();
        Console.WriteLine("All professional chess piece images generated successfully!");
    }

    static void GenerateRealisticPawn(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            using (GraphicsPath basePath = new GraphicsPath())
            {
                basePath.AddEllipse(centerX - 18, centerY + 15, 36, 8);
                using (PathGradientBrush baseGrad = new PathGradientBrush(basePath))
                {
                    baseGrad.CenterColor = mainColor;
                    baseGrad.SurroundColors = new Color[] { darkColor };
                    g.FillPath(baseGrad, basePath);
                }
            }

            // Stem
            Rectangle stemRect = new Rectangle(centerX - 10, centerY - 10, 20, 25);
            using (LinearGradientBrush stemGrad = new LinearGradientBrush(stemRect, lightColor, darkColor, LinearGradientMode.Horizontal))
            {
                g.FillEllipse(stemGrad, stemRect);
            }
            g.DrawEllipse(new Pen(darkColor, 2), stemRect);

            // Head
            Rectangle headRect = new Rectangle(centerX - 14, centerY - 25, 28, 28);
            using (LinearGradientBrush headGrad = new LinearGradientBrush(headRect, lightColor, mainColor, 45f))
            {
                g.FillEllipse(headGrad, headRect);
            }
            g.DrawEllipse(new Pen(darkColor, 2), headRect);

            // Highlight
            if (isWhite)
            {
                using (SolidBrush highlight = new SolidBrush(Color.FromArgb(100, 255, 255, 255)))
                {
                    g.FillEllipse(highlight, centerX - 8, centerY - 22, 12, 10);
                }
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRealisticRook(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            Rectangle baseRect = new Rectangle(centerX - 20, centerY + 10, 40, 12);
            using (LinearGradientBrush baseGrad = new LinearGradientBrush(baseRect, mainColor, darkColor, LinearGradientMode.Vertical))
            {
                g.FillRectangle(baseGrad, baseRect);
            }

            // Main tower body
            Rectangle bodyRect = new Rectangle(centerX - 16, centerY - 15, 32, 25);
            using (LinearGradientBrush bodyGrad = new LinearGradientBrush(bodyRect, lightColor, darkColor, LinearGradientMode.Horizontal))
            {
                g.FillRectangle(bodyGrad, bodyRect);
            }
            g.DrawRectangle(new Pen(darkColor, 2), bodyRect);

            // Battlements (castle top)
            for (int i = 0; i < 3; i++)
            {
                Rectangle battlement = new Rectangle(centerX - 14 + i * 12, centerY - 25, 10, 10);
                using (LinearGradientBrush battleGrad = new LinearGradientBrush(battlement, lightColor, mainColor, LinearGradientMode.Vertical))
                {
                    g.FillRectangle(battleGrad, battlement);
                }
                g.DrawRectangle(new Pen(darkColor, 1.5f), battlement);
            }

            // Highlight
            if (isWhite)
            {
                using (SolidBrush highlight = new SolidBrush(Color.FromArgb(80, 255, 255, 255)))
                {
                    g.FillRectangle(highlight, centerX - 14, centerY - 10, 8, 15);
                }
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRealisticKnight(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            Rectangle baseRect = new Rectangle(centerX - 18, centerY + 12, 36, 10);
            using (LinearGradientBrush baseGrad = new LinearGradientBrush(baseRect, mainColor, darkColor, LinearGradientMode.Vertical))
            {
                g.FillEllipse(baseGrad, baseRect);
            }

            // Horse head shape
            Point[] horseHead = new Point[]
            {
                new Point(centerX - 8, centerY + 12),   // Bottom left
                new Point(centerX - 10, centerY - 5),   // Left mid
                new Point(centerX - 5, centerY - 18),   // Ear
                new Point(centerX + 8, centerY - 15),   // Top of head
                new Point(centerX + 15, centerY - 8),   // Nose
                new Point(centerX + 12, centerY + 0),   // Jaw
                new Point(centerX + 8, centerY + 12)    // Bottom right
            };

            using (GraphicsPath horsePath = new GraphicsPath())
            {
                horsePath.AddCurve(horseHead, 0.5f);
                horsePath.CloseFigure();

                using (PathGradientBrush horseGrad = new PathGradientBrush(horsePath))
                {
                    horseGrad.CenterColor = lightColor;
                    horseGrad.SurroundColors = new Color[] { darkColor, darkColor, mainColor, darkColor, darkColor, darkColor, darkColor };
                    g.FillPath(horseGrad, horsePath);
                }
                g.DrawPath(new Pen(darkColor, 2), horsePath);
            }

            // Eye
            g.FillEllipse(new SolidBrush(darkColor), centerX + 5, centerY - 10, 4, 4);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRealisticBishop(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            Rectangle baseRect = new Rectangle(centerX - 18, centerY + 12, 36, 10);
            using (LinearGradientBrush baseGrad = new LinearGradientBrush(baseRect, mainColor, darkColor, LinearGradientMode.Vertical))
            {
                g.FillEllipse(baseGrad, baseRect);
            }

            // Lower body (trapezoid shape)
            Point[] bodyPoints = new Point[]
            {
                new Point(centerX - 15, centerY + 12),
                new Point(centerX - 10, centerY - 8),
                new Point(centerX + 10, centerY - 8),
                new Point(centerX + 15, centerY + 12)
            };

            using (LinearGradientBrush bodyGrad = new LinearGradientBrush(
                new Rectangle(centerX - 15, centerY - 8, 30, 20),
                lightColor, darkColor, LinearGradientMode.Horizontal))
            {
                g.FillPolygon(bodyGrad, bodyPoints);
            }
            g.DrawPolygon(new Pen(darkColor, 2), bodyPoints);

            // Top sphere with slit
            Rectangle topRect = new Rectangle(centerX - 10, centerY - 22, 20, 20);
            using (LinearGradientBrush topGrad = new LinearGradientBrush(topRect, lightColor, mainColor, 45f))
            {
                g.FillEllipse(topGrad, topRect);
            }
            g.DrawEllipse(new Pen(darkColor, 2), topRect);

            // Slit in top
            g.DrawLine(new Pen(darkColor, 3), centerX - 3, centerY - 18, centerX + 3, centerY - 18);

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRealisticQueen(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            Rectangle baseRect = new Rectangle(centerX - 20, centerY + 12, 40, 10);
            using (LinearGradientBrush baseGrad = new LinearGradientBrush(baseRect, mainColor, darkColor, LinearGradientMode.Vertical))
            {
                g.FillEllipse(baseGrad, baseRect);
            }

            // Main body
            Rectangle bodyRect = new Rectangle(centerX - 18, centerY - 12, 36, 36);
            using (LinearGradientBrush bodyGrad = new LinearGradientBrush(bodyRect, lightColor, darkColor, 45f))
            {
                g.FillEllipse(bodyGrad, bodyRect);
            }
            g.DrawEllipse(new Pen(darkColor, 2), bodyRect);

            // Crown points (5 points)
            for (int i = 0; i < 5; i++)
            {
                double angle = i * 72.0 - 90.0;
                float radius = 24.0f;
                float px = centerX + (float)(radius * Math.Cos(angle * Math.PI / 180.0));
                float py = centerY - 5 + (float)(radius * Math.Sin(angle * Math.PI / 180.0));

                using (LinearGradientBrush pointGrad = new LinearGradientBrush(
                    new RectangleF(px - 5, py - 5, 10, 10),
                    lightColor, darkColor, LinearGradientMode.Vertical))
                {
                    g.FillEllipse(pointGrad, px - 5, py - 5, 10, 10);
                }
                g.DrawEllipse(new Pen(darkColor, 1.5f), px - 5, py - 5, 10, 10);
            }

            // Highlight
            if (isWhite)
            {
                using (SolidBrush highlight = new SolidBrush(Color.FromArgb(100, 255, 255, 255)))
                {
                    g.FillEllipse(highlight, centerX - 10, centerY - 8, 15, 12);
                }
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }

    static void GenerateRealisticKing(string dir, string filename, bool isWhite, int size)
    {
        using (Bitmap bmp = new Bitmap(size, size))
        using (Graphics g = Graphics.FromImage(bmp))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(Color.Transparent);

            int centerX = size / 2;
            int centerY = size / 2;

            Color mainColor = isWhite ? Color.FromArgb(240, 240, 240) : Color.FromArgb(60, 60, 60);
            Color darkColor = isWhite ? Color.FromArgb(180, 180, 180) : Color.FromArgb(30, 30, 30);
            Color lightColor = isWhite ? Color.White : Color.FromArgb(90, 90, 90);

            // Base
            Rectangle baseRect = new Rectangle(centerX - 20, centerY + 12, 40, 10);
            using (LinearGradientBrush baseGrad = new LinearGradientBrush(baseRect, mainColor, darkColor, LinearGradientMode.Vertical))
            {
                g.FillEllipse(baseGrad, baseRect);
            }

            // Main body
            Rectangle bodyRect = new Rectangle(centerX - 16, centerY - 10, 32, 32);
            using (LinearGradientBrush bodyGrad = new LinearGradientBrush(bodyRect, lightColor, darkColor, 45f))
            {
                g.FillEllipse(bodyGrad, bodyRect);
            }
            g.DrawEllipse(new Pen(darkColor, 2), bodyRect);

            // Cross on top
            Rectangle crossVRect = new Rectangle(centerX - 3, centerY - 32, 6, 20);
            Rectangle crossHRect = new Rectangle(centerX - 10, centerY - 25, 20, 6);

            using (LinearGradientBrush crossGrad = new LinearGradientBrush(crossVRect, lightColor, mainColor, LinearGradientMode.Vertical))
            {
                g.FillRectangle(crossGrad, crossVRect);
                g.FillRectangle(crossGrad, crossHRect);
            }
            g.DrawRectangle(new Pen(darkColor, 2), crossVRect);
            g.DrawRectangle(new Pen(darkColor, 2), crossHRect);

            // Highlight
            if (isWhite)
            {
                using (SolidBrush highlight = new SolidBrush(Color.FromArgb(100, 255, 255, 255)))
                {
                    g.FillEllipse(highlight, centerX - 10, centerY - 6, 15, 12);
                }
            }

            bmp.Save(System.IO.Path.Combine(dir, filename), ImageFormat.Png);
            Console.WriteLine($"Created: {filename}");
        }
    }
}
