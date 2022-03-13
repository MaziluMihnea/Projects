package Workers;
import java.awt.image.BufferedImage;

public class Buffer { //clasa Buffer
	private BufferedImage img;//imaginea continuta in buffer
	
	public Buffer(){     //constructor
		this.img = null; //cand e construit prima data bufferul nu are nici o imagine in el deci => null
	}
	
	public void set(BufferedImage img){ //metoda de set a bufferului
		this.img = img; //seteaza atributul clasei cu argumentul metodei
	}
	
	public BufferedImage get(){			//metoda de get a bufferului
		return img;						//returneaza un obiect de tip BufferedImage
	}
}
