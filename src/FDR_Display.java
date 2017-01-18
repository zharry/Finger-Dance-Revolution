import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class FDR_Display {

	// Game Variables
	static final String TITLE = "Finger Dance Revolution";
	static int width = 600, height = 480, panelWidth, panelHeight;

	static final int[] xCOORDS = { 0, 0, 50, 100, 150, 200, 360, 410, 460, 510 };
	static final int TEXTLOC = 250;
	static int p1Score = 0, p2Score = 0;
	static int mili = 0, seconds = 0;
	static ArrayList<ArrayList<Integer>> arrows = new ArrayList<ArrayList<Integer>>();

	static JPanel gamePanel;
	static JFrame frame;
	static boolean running;
	static boolean gameOver = false;
	static String winner;

	static BufferedImage sprLeft, sprRight, sprTop, sprDown;
	static BufferedImage sprLeftB, sprRightB, sprTopB, sprDownB;

	static File serial;
	static BufferedReader s;

	static Font normal = new Font(Font.SANS_SERIF, Font.BOLD, 12);
	static Font gameOverFont = new Font(Font.SANS_SERIF, Font.BOLD, 25);

	static File music;

	public static void main(String[] args) throws Exception {

		Runtime.getRuntime().exec("screen /dev/ttyAMA0 115200").destroy();

		for (int i = 0; i < 10; i++)
			arrows.add(new ArrayList<Integer>());

		sprLeft = ImageIO.read(new File("Left.png"));
		sprRight = ImageIO.read(new File("Right.png"));
		sprTop = ImageIO.read(new File("Top.png"));
		sprDown = ImageIO.read(new File("Down.png"));

		sprLeftB = ImageIO.read(new File("LeftBlank.png"));
		sprRightB = ImageIO.read(new File("RightBlank.png"));
		sprTopB = ImageIO.read(new File("TopBlank.png"));
		sprDownB = ImageIO.read(new File("DownBlank.png"));

		music = new File("Song.mp3");
		AudioInputStream stream = AudioSystem.getAudioInputStream(music);
		Clip clip = AudioSystem.getClip();
	    clip.open(stream);

		serial = new File("/dev/ttyAMA0");
		s = new BufferedReader(new InputStreamReader(new FileInputStream(serial)), 8);

		// Start Game
		running = true;
		createWindow();

		// Game Loop
		new Thread() {
			public void run() {
				while (true) {
					try {
						String commands = s.readLine();
						arrows.removeAll(arrows);
						for (int i = 0; i < 10; i++)
							arrows.add(new ArrayList<Integer>());
						String[] cmdList = commands.split("\\+");
						for (String cmd : cmdList) {
							String[] proc = cmd.split(":");
							String c = proc[0];
							String[] nums = proc[1].split(",");
							int button = Integer.parseInt(nums[0]);
							int y = Integer.parseInt(nums[1]);
							if (c.equals("L") || c.equals("R") || c.equals("T") || c.equals("B")) {
								arrows.get(button).add(y);
							} else if (c.equals("CT")) {
								mili = button % 1000;
								seconds = button / 1000;
							} else if (c.equals("MN")) {
								System.out.println("Missed " + button);
							} else if (c.equals("SO")) {
								System.out.println("Game Over!");
								System.out.println("Player 1: " + button);
								System.out.println("Player 2: " + y);
								gameOver = true;
								winner = button > y ? "Player 1 Wins!" : (button == y ? "Tie!" : "Player 2 Wins!");
							    clip.stop();
							} else if (c.equals("DPA")) {
								System.out.println("Double Points " + button);
							} else if (c.equals("FPA")) {
								System.out.println("Full Points " + button);
							} else if (c.equals("HPA")) {
								System.out.println("Half Points " + button);
							} else if (c.equals("NPA")) {
								System.out.println("No Points " + button);
							} else if (c.equals("SC")) {
								p1Score = button;
								p2Score = y;
							} else if (c.equals("SS")) {
								gameOver = false;
							    clip.start();
							}
						}
					} catch (Exception e) {
					}
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
		int textY = 20, textIncY = 20;

		// Render Game
		g.drawImage(sprLeftB, xCOORDS[2], 210, null);
		g.drawImage(sprTopB, xCOORDS[3], 210, null);
		g.drawImage(sprDownB, xCOORDS[4], 210, null);
		g.drawImage(sprRightB, xCOORDS[5], 210, null);
		g.drawImage(sprLeftB, xCOORDS[6], 210, null);
		g.drawImage(sprTopB, xCOORDS[7], 210, null);
		g.drawImage(sprDownB, xCOORDS[8], 210, null);
		g.drawImage(sprRightB, xCOORDS[9], 210, null);

		for (int i = 0; i < arrows.get(2).size(); i++)
			g.drawImage(sprLeft, xCOORDS[2], arrows.get(2).get(i), null);
		for (int i = 0; i < arrows.get(3).size(); i++)
			g.drawImage(sprTop, xCOORDS[3], arrows.get(3).get(i), null);
		for (int i = 0; i < arrows.get(4).size(); i++)
			g.drawImage(sprDown, xCOORDS[4], arrows.get(4).get(i), null);
		for (int i = 0; i < arrows.get(5).size(); i++)
			g.drawImage(sprRight, xCOORDS[5], arrows.get(5).get(i), null);
		for (int i = 0; i < arrows.get(6).size(); i++)
			g.drawImage(sprLeft, xCOORDS[6], arrows.get(6).get(i), null);
		for (int i = 0; i < arrows.get(7).size(); i++)
			g.drawImage(sprTop, xCOORDS[7], arrows.get(7).get(i), null);
		for (int i = 0; i < arrows.get(8).size(); i++)
			g.drawImage(sprDown, xCOORDS[8], arrows.get(8).get(i), null);
		for (int i = 0; i < arrows.get(9).size(); i++)
			g.drawImage(sprRight, xCOORDS[9], arrows.get(9).get(i), null);

		g.setColor(Color.BLACK);
		g.setFont(normal);
		g.drawString("Player 1: " + p1Score, TEXTLOC, textY += textIncY);
		g.drawString("Player 2: " + p2Score, TEXTLOC, textY += textIncY);
		g.drawString("Song Time: ", TEXTLOC, textY += textIncY);
		g.drawString(seconds + ":" + mili, TEXTLOC, textY += textIncY);

		if (gameOver) {
			g.setColor(Color.BLUE);
			g.setFont(gameOverFont);
			g.drawString("Game Over!", TEXTLOC - 40, textY += (textIncY * 2));
			g.drawString(winner, TEXTLOC - 40, textY += (textIncY * 2));
		}

	}

}