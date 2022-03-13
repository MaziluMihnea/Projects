package Main;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class PowerLawGrayT { //clasa pentru functia Power Law Gray Level Transform
	private static int rgb;  //variabila ce va retine intr-un int rgb ul fiecarui pixel
	private static int gray; //variabila ce va retine doar o valoare intre 0 - 255 pentru gri
	
	public void GrayTransform(BufferedImage image, double gamma, String path) throws IOException {	//constructor

		//pentru fiecare pixel transforma acel pixel conform regulii Power Law
	    for (int xPixel = 0; xPixel < image.getWidth(); xPixel++) {
	    	for (int yPixel = 0; yPixel < image.getHeight(); yPixel++) {
	    		
	    		rgb = image.getRGB(xPixel, yPixel);  //preia un int cu valoriile R G B
	    		gray = rgb & 0xff;					 //in gray se vor salva doar 8 biti ce ne trebuie ca sa stim valoarea de gri
	    		gray =(int) Math.floor(255 * Math.pow(1.0 * gray/255, gamma));  //transformare gamma
	    		rgb = (0x00 << 24) + (gray << 16) + (gray << 8) + gray;  // refacem int ul rgb
	    		image.setRGB(xPixel, yPixel, rgb);   //setam pixelul in formatul cel nou
	         }
	     }
	     ImageIO.write(image, "BMP", new File(path));	 //scriem in fisier imaginea nou obtinuta    
	}
}
