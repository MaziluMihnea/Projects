package Workers;

import java.util.concurrent.Semaphore;

import Main.MainClass;

public class Consumer extends Worker{ //extinde clasa abstracta Worker
	private Semaphore sem;  //semafor pentru sincronizare threaduri
  
    public Consumer(Buffer img, Semaphore sem)  //constructor Consumer
    {
        this.img = img;
        this.sem = sem;
    }

    @Override
    public void run() {
    	synchronized(img){    //Threadul Consumer este sincronizat si el tot pe obiectul img mostenit din clasa Worker
	    	try{
	    		sem.release();  //Consumer anunta Producer ca a intrat in zona critica
	    		int line = 0;  //nr liniei la care ne aflam la momentul actual cand se scrie imaginea
	    		for(int i = 0; i < 4; i++) {  //rulam de 4 ori pentru fiecare sfert
	    			
	    			img.wait(); // Consumer asteapta notify() pe obiectul comun img de la Producer
	    			
	    			//se scrie in finalImage din clasa Main pe rand de catre Consumer cate un sfert de imagine
	    	        for(int j = 0; j < img.get().getHeight(); j ++){
	    	        	for(int k = 0; k < img.get().getWidth(); k++){
	    	        		MainClass.finalImage.setRGB(k,  line,  img.get().getRGB(k,  j));// fiecare pixel din finalImage se seteaza conform buffer-ului
	    	        	}
	    	        	line ++;
	    	        }
	    	        
	    	        System.out.println("Consumed " + (i + 1) + "/4"); //afisare a cata parte a fost consumata
		       		Thread.sleep(100); //sleep pentru a observa rularea
		       		img.notifyAll();   //notificam Threadul Producer ca am terminat de consumat informatia
	    		}	
	        } catch (InterruptedException e) { //tratarea exceptiilor
				e.printStackTrace();
			}
    	}
    }
}
