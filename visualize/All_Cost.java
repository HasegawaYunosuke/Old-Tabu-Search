import java.net.*;
import java.io.*;
import java.util.*;
import javax.swing.* ;
import java.awt.Graphics ;
import java.awt.Color ;
import javax.swing.JPanel;
import javax.swing.JFrame;
import java.awt.Container;
import java.awt.BorderLayout;

class All_Cost extends JPanel{
    
    private int solution[] = new int [30];
	private boolean flag = false;
	private int i = 0;
	private int x = 0;
	private int kazu = 0;
	private int oox,ooy;
	private int bbx = 900,bby = 400;
    private int waru = 0;
	String msg;
	String[] msg_split;
	String line2;
	JFrame frame;
	java.awt.Rectangle r;

	public All_Cost(int mx,int my){
        frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setBounds(mx+500, my, 300, 300);
		frame.setTitle("遷移解");
		frame.getContentPane().add(this);
		frame.setVisible(true);
	}

	private int getReaX( int xx){
		return oox + xx;
	}

	private int getReaY( int yy){

        waru = yy / 280;
		return ooy - waru;
	}

	public void Re_show(int now_number,int max, String Aname){
        
        try{
            
            FileInputStream grand = new FileInputStream(Aname);
			InputStreamReader phicnd = new InputStreamReader(grand, "EUC-JP");
			BufferedReader brnd = new BufferedReader(phicnd);

			msg = brnd.readLine();
			this.i = 0;
			this.kazu = 0;

			if(now_number-1 < 30){
                while( (line2 = brnd.readLine()) != null){
                    if(kazu < now_number-1){
                        msg_split = line2.split(",",0);
						solution[this.i] = Integer.parseInt(msg_split[max+1]);
						this.i++;
                        if(now_number-1 == 30){
                            break;
                        }
					}
                    kazu++;
				}
			}
			else{
                while( (line2 = brnd.readLine()) != null){
					if(kazu > now_number-31 && kazu < now_number-1){
						msg_split = line2.split(",",0);
						solution[this.i] = Integer.parseInt(msg_split[max+1]);
						this.i++;
					}
					kazu++;
                    if(this.i == 30){
                        break;
                    }
                }
			}

			brnd.close();
			phicnd.close();
			grand.close();
            
        }catch (IOException e){
		}
        this.flag = true;
        frame.repaint();
	}

    public void clean_frame(){
        frame.dispose();
    }

	public void paintComponent(Graphics g){
        this.r = getBounds();
        this.oox = 0;
        this.ooy = r.height/2;
		this.bbx = r.width;
		this.bby = r.height;
		super.paintComponent(g);
		g.setColor(Color.red);
		this.x = 0;

		if(flag == true){
            for(int m=0;m<this.i-1;m++){
                g.drawLine(this.x,getReaY(this.solution[m]/500),this.x+10,getReaY(this.solution[m+1]/500) );
                this.x += 7;
			}
		}
	}
}
