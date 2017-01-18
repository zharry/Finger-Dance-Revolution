import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class FDR_Display {

	// Game Variables
	static final String TITLE = "Finger Dance Revolution";
	static int width = 600, height = 480, panelWidth, panelHeight;

	static String commands = "";
	static boolean processed = false;

	static final int[] xCOORDS = { 0, 0, 50, 100, 150, 200, 360, 410, 460, 510 };
	static final int TEXTLOC = 250;
	static int p1Score = 0, p2Score = 0;
	static int mili = 0, seconds = 0;

	static JPanel gamePanel;
	static JFrame frame;
	static boolean running;

	static BufferedImage sprLeft, sprRight, sprTop, sprDown;
	static BufferedImage sprLeftB, sprRightB, sprTopB, sprDownB;

	static File serial;
	static InputStream s;

	public static void main(String[] args) throws Exception {

		sprLeft = ImageIO.read(new File("Left.png"));
		sprRight = ImageIO.read(new File("Right.png"));
		sprTop = ImageIO.read(new File("Top.png"));
		sprDown = ImageIO.read(new File("Down.png"));

		sprLeftB = ImageIO.read(new File("LeftBlank.png"));
		sprRightB = ImageIO.read(new File("RightBlank.png"));
		sprTopB = ImageIO.read(new File("TopBlank.png"));
		sprDownB = ImageIO.read(new File("DownBlank.png"));

		serial = new File("/dev/ttyAMA0");
		s = new FileInputStream(serial);

		// Start Game
		running = true;
		createWindow();

		// Game Loop
		new Thread() {
			public void run() {
				try {
					byte[] buffer = new byte[1024];
					int c;
					while((c = s.read(buffer, 0, 1024)) != -1) {
					    System.out.print(new String(buffer, 0, c));
					}
				} catch (IOException e) {
				}
			}
		}.start();
		while (running) {
			gamePanel.repaint();
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
				try {
					render(g);
				} catch (Exception e) {
				}
			}
		};

		frame.add(gamePanel);
		frame.setVisible(true);

		panelWidth = gamePanel.getWidth();
		panelHeight = gamePanel.getHeight();
	}

	static void render(Graphics g) throws Exception {
		int textY = 0, textIncY = 20;

		// Render Game
		g.drawImage(sprLeftB, xCOORDS[2], 210, null);
		g.drawImage(sprTopB, xCOORDS[3], 210, null);
		g.drawImage(sprDownB, xCOORDS[4], 210, null);
		g.drawImage(sprRightB, xCOORDS[5], 210, null);
		g.drawImage(sprLeftB, xCOORDS[6], 210, null);
		g.drawImage(sprTopB, xCOORDS[7], 210, null);
		g.drawImage(sprDownB, xCOORDS[8], 210, null);
		g.drawImage(sprRightB, xCOORDS[9], 210, null);

		//System.out.println(commands);
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
				mili = button % 1000;
				seconds = button / 1000;
			} else if (c.equals("MN")) {
			//	System.out.println("Missed " + button);
			} else if (c.equals("SO")) {
			//	System.out.println("Game Over!");
			//	System.out.println("Player 1: " + button);
			//	System.out.println("Player 2: " + y);
			} else if (c.equals("DPA")) {
			//	System.out.println("Double Points " + button);
			} else if (c.equals("FPA")) {
			//	System.out.println("Full Points  " + button);
			} else if (c.equals("HPA")) {
			//	System.out.println("Half Points " + button);
			} else if (c.equals("NPA")) {
			//	System.out.println("No Points " + button);
			} else if (c.equals("SC")) {
				g.setColor(Color.black);
				p1Score = button;
				p2Score = y;
			}
		}

		processed = true;

		g.drawString("Player 1: " + p1Score, TEXTLOC, textY += textIncY);
		g.drawString("Player 2: " + p2Score, TEXTLOC, textY += textIncY);
		g.drawString("Song Time: ", TEXTLOC, textY += textIncY);
		g.drawString(seconds + ":" + mili, TEXTLOC, textY += textIncY);

	}

}