import java.io.*;
import java.util.Arrays;

public class Sol2 {
	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_WHITE = "\u001B[37m";

	public static void main(String[] args) throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader("input"));
		String input = reader.readLine();
		reader.close();

		int width = 25;
		int height = 6;
		int layers = input.length() / (width * height);

		/* For this part we skip the layer system altogether, and just overlap
		 * pixels into a 2D image
		 * */
		char img[][] = new char[height][width];

		// Fill the image with transparency
		for (int i=0; i<height; i++){
			Arrays.fill(img[i], '2');
		}

		for(int z=0; z<layers; z++) {
			for(int y=0; y<height; y++) {
				for(int x=0; x<width; x++) {
					int position = z*width*height + y*width + x;
					char read = input.charAt(position);
					// System.out.println(String.format("img[%d][%d][%d] = input[%d] (%c)", z, y, x, position, read));
					if(read != '2' && img[y][x] == '2'){
						// Overlap our pixel
						img[y][x] = read;
					}
					if(z == layers-1){
						// If we are already at the last layer, print the top pixel
						char pixel = img[y][x];
						if(pixel == '2') System.out.print(' ');
						else{
							// I'm choosing red and green because they are
							// easier to see in my black-on-white terminal
							String color = ANSI_WHITE;
							if(pixel == '0') color = ANSI_RED;
							else if(pixel == '1') color = ANSI_GREEN;
							System.out.print(color + pixel + ANSI_RESET);
						}
					}
				}
				if(z == layers-1) System.out.print('\n');
			}
		}
	}
}
