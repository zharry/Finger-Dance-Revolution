import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class FDR_Display {

	// Game Variables
	static final String TITLE = "Finger Dance Revolution";
	static int width = 800, height = 480, panelWidth, panelHeight;
	static int fps, fpsProc = 0, tps = 60, curTps;

	static String commands = "R:5,215+CT:34464,0+SC:100,0+";
	static final int[] xCOORDS = { 0, 0, 50, 100, 150, 200, 560, 610, 660, 710 };
	static int p1Score = 0, p2Score = 0;

	static JPanel gamePanel;
	static JFrame frame;
	static boolean running;

	static BufferedImage sprLeft, sprRight, sprTop, sprDown;
	static BufferedImage sprLeftB, sprRightB, sprTopB, sprDownB;

	public static void main(String[] args) throws Exception {

		sprLeft = ImageIO.read(new File("Left.png"));
		sprRight = ImageIO.read(new File("Right.png"));
		sprTop = ImageIO.read(new File("Top.png"));
		sprDown = ImageIO.read(new File("Down.png"));

		sprLeftB = ImageIO.read(new File("LeftBlank.png"));
		sprRightB = ImageIO.read(new File("RightBlank.png"));
		sprTopB = ImageIO.read(new File("TopBlank.png"));
		sprDownB = ImageIO.read(new File("DownBlank.png"));
		
		// Start Game
		running = true;
		createWindow();

		System.out.println(panelHeight);
		// Game Loop
		long lastTime = System.nanoTime(), timer = System.currentTimeMillis();
		double ns = 1000000000 / (double) tps, delta = 0;
		int tpsProc = 0;
		while (running) {
			long curTime = System.nanoTime();
			delta += (curTime - lastTime) / ns;
			lastTime = curTime;
			while (delta >= 1) {
				// Process Game Changes
				tick();
				tpsProc++;
				delta--;
			}
			// Update the Graphics
			gamePanel.repaint();
			// Display FPS and TPS
			if (System.currentTimeMillis() - timer > 1000) {
				timer += 1000;
				fps = fpsProc;
				curTps = tpsProc;
				fpsProc = 0;
				tpsProc = 0;
			}
			Thread.sleep(1);
		}
	}

	@SuppressWarnings("serial")
	static void createWindow() {
		frame = new JFrame(TITLE);
		frame.setPreferredSize(new Dimension(width, height));
		frame.setMaximumSize(new Dimension(width, height));
		frame.setMinimumSize(new Dimension(width, height));
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);

		gamePanel = new JPanel() {
			@Override
			public void paint(Graphics g) {
				// Reset Frame
				g.setColor(Color.lightGray);
				g.fillRect(0, 0, getWidth(), getHeight());
				// Render Game
				render(g);
				fpsProc++;
			}
		};

		frame.add(gamePanel);
		frame.setVisible(true);

		panelWidth = gamePanel.getWidth();
		panelHeight = gamePanel.getHeight();
	}

	static void tick() {
		
	}

	static void render(Graphics g) {
		// Draw Debug
		g.setColor(Color.black);
		g.drawString("FPS: " + fps + " TPS: " + tps, 320, 20);

		// Render Game
		//g.setColor(Color.magenta);
		//g.drawLine(0, 209, width, 209);
		g.drawImage(sprLeftB, xCOORDS[2], 210, null);
		g.drawImage(sprTopB, xCOORDS[3], 210, null);
		g.drawImage(sprDownB, xCOORDS[4], 210, null);
		g.drawImage(sprRightB, xCOORDS[5], 210, null);
		g.drawImage(sprLeftB, xCOORDS[6], 210, null);
		g.drawImage(sprTopB, xCOORDS[7], 210, null);
		g.drawImage(sprDownB, xCOORDS[8], 210, null);
		g.drawImage(sprRightB, xCOORDS[9], 210, null);
		
		String[] cmdList = commands.split("\\+");
		for (String cmd : cmdList) {
			String[] proc = cmd.split(":");
			String c = proc[0];
			String[] nums = proc[1].split(",");
			int button = Integer.parseInt(nums[0]);
			int y = Integer.parseInt(nums[1]);
			if (c.equals("L")) {
				g.drawImage(sprLeft, xCOORDS[button], y, null);
			} else if (c.equals("R")) {
				g.drawImage(sprRight, xCOORDS[button], y, null);
			} else if (c.equals("T")) {
				g.drawImage(sprTop, xCOORDS[button], y, null);
			} else if (c.equals("B")) {
				g.drawImage(sprDown, xCOORDS[button], y, null);
			} else if (c.equals("CT")) {
				g.setColor(Color.black);
				int mili = button % 1000;
				int seconds = button / 1000;
				g.drawString("Song Time: " + seconds + ":" + mili, 320, 40);
			} else if (c.equals("MN")) {
				System.out.println("Missed " + button);
			} else if (c.equals("SO")) {
				System.out.println("Game Over!");
				System.out.println("Player 1: " + button);
				System.out.println("Player 2: " + y);
			} else if (c.equals("DPA")) {
				System.out.println("Double Points " + button);
			} else if (c.equals("FPA")) {
				System.out.println("Full Points  " + button);
			} else if (c.equals("HPA")) {
				System.out.println("Half Points " + button);
			} else if (c.equals("NPA")) {
				System.out.println("No Points " + button);
			} else if (c.equals("SC")) {
				g.setColor(Color.black);
				p1Score = button;
				p2Score = y;
				g.drawString("Player 1: " + p1Score, 320, 60);
				g.drawString("Player 2: " + p2Score, 320, 80);
			}
		}

	}

}
