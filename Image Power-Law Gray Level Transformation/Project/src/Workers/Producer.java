package Workers;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.concurrent.Semaphore;
import javax.imageio.ImageIO;
import Main.MainClass;

public class Producer extends Worker {    //extinde clasa abstracta Worker
    private BufferedImage initialImage = null; //initializam locul unde se va citi din fisier
    private String path; 					   //initializam cu o variabila path ul ce va fi dat pentru citire
    private Semaphore sem;					   //semafor pentru sincronizarea threadurilor
    
    public Producer(Buffer img, String path, Semaphore sem) //constructor Producer
    {
        this.img = img;
        this.path = path;
        this.sem = sem;
    }
    
    @Override
	public void run()
    {	
    	try {
			sem.acquire(); //threadul il lasa mai intai pe Consumer sa intre in zona critica
	    	synchronized(img){	//Threadul Producer este sincronizat pe obiectul img mostenit din calsa Worker
		    		initialImage = ImageIO.read(new FileInputStream(path)); //citim poza din fisier in initialImage
		    		MainClass.finalImage = new BufferedImage(initialImage.getWidth(), initialImage.getHeight(), initialImage.getType()); 	//initializam dimensiuniile variabilei care va contine poza finala cu dimensiuniile pozei citite
		    		for(int i = 0; i < 4; i++) { 	//rulam de 4 ori, pentru fiecare sfert
		    			
		    			//cu ajutorul variabilelor start si end vom imparti BufferedImage ul in 4
		    			int start = (int) Math.round(i * (double)initialImage.getHeight() / 4);
		    	        int end = Math.min((int) Math.round((i + 1) * (double)initialImage.getHeight() /4), initialImage.getHeight());	    	        
		    	        BufferedImage aux = initialImage.getSubimage(0, start, initialImage.getWidth(), end - start); 	//cream o variabila aux ce va retine cate un sfert din imaginea totala
		    	              
		    	        img.set(aux);	   //salvam in buffer sfertul din imagine
			       		System.out.println("Produced "+ (i + 1) +"/4"); //afiseaza a cata parte s-a trimis
			       		Thread.sleep(100); //sleep pentru a se observa rularea
			       		img.notifyAll();   //notificam Threadul Consumer ca Producer a trmis sfertul de imagine
			       		img.wait();        //Producer intra in asteptare
		    		}
	    	}
    	} catch (IOException e) {   //tratare exceptii
				e.printStackTrace();
		} catch (InterruptedException e) {
				e.printStackTrace();
		}
    }
}
