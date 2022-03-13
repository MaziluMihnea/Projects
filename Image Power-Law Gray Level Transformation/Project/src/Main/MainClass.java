package Main;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.concurrent.Semaphore;

import Picture.Picture;
import Workers.Buffer;
import Workers.Consumer;
import Workers.Producer;
import Workers.Worker;

public class MainClass {
	public static BufferedImage finalImage; //variabila pentru imaginea finala
	public static Semaphore sem = new Semaphore(0); //semafor pentru inceperea threadurilor

	public static void main(String[] args) {
	
		Picture cache[] = new Picture[args.length - 2]; //initializam cache ul
		PowerLawGrayT func = new PowerLawGrayT();       //un obiect ce va transforma imaginea
		Buffer buf = new Buffer();;						//obiectul buffer
		Worker p,c;										//cei 2 workeri
		
		
		for(int i = 2; i < args.length ; i++){    //rula de atatea ori de cate parh uri avem in linia de comanda
			
			p = new Producer(buf, args[i], sem);		  //initializam un Producer
			c = new Consumer(buf, sem);				  //si un Consumer
			
			try {
				c.start();						  //pornim Consumer si Producer
			    p.start();
		    
		 
		    
		        p.join();						  //reunim Threadurile Producer Consumer
		        c.join();
		        
		        String out_path = args[1] + "/Modified" + (i-1) +".bmp";  //obtinem path ul in care se va scrie imaginea
		        func.GrayTransform(finalImage, Double.parseDouble(args[0]), out_path);  //transformare imagine + scriere in fisier
		    } catch (InterruptedException e) { // tratare exceptii
		        e.printStackTrace();
		    } catch (NumberFormatException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			//formarea descrierii unei poze, contrast inaltime etc
			String descript[][] = {{"Contrast: ",args[0]},{"Inaltime: ",String.valueOf(finalImage.getHeight())},{"Latime: ",String.valueOf(finalImage.getWidth())}};
			cache[i-2] = new Picture(descript, finalImage);//salvare imagine in cache
			System.out.println("Processed Image " + (i-1) + " " + cache[i-2].getHeight()+ "x" + cache[i-2].getWidth());
		}
		
		System.out.println();
		display(cache.length, cache); // afisare cache
	}
	
	public static void display(int n, Picture[] img){ //functie pentru display cache
		
		for(int i =0; i < n; i ++){
			System.out.println("Descriere Imagine " + (i+1) + ":");
			img[i].display(); // afisarea descrierii unei imagini
			System.out.println();
		}
	}
}
