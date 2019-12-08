import java.io.*;

public class Sol1 {
	public static void main(String[] args) throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader("input"));
		String input = reader.readLine();
		reader.close();

		int width = 25;
		int height = 6;
		int layers = input.length() / (width * height);
		System.out.println( "layers: " + layers );

		int fewest_zeroes = 99999999;
		int layer_value = 0;  // number of ones by number of twos
		int img[][][] = new int[layers][height][width];
		for(int z=0; z<layers; z++) {
			int zeroes = 0, ones = 0, twos = 0;
			for(int y=0; y<height; y++) {
				for(int x=0; x<width; x++) {
					char read = input.charAt(z*width*height + y*width + x);
					if(read == '0') zeroes++;
					if(read == '1') ones++;
					if(read == '2') twos++;
					img[z][y][x] = read;
					// System.out.println(String.format("img[%d][%d][%d] = input[%d] (%c)", z, y, x, z*width*height + y*width + x, read));
				}
			}
			if(zeroes < fewest_zeroes){
				fewest_zeroes = zeroes;
				layer_value = ones * twos;
			}
		}

		System.out.println("Fewest zeroes: " + fewest_zeroes);
		System.out.println("Ones by twos: " + layer_value);
	}
}

