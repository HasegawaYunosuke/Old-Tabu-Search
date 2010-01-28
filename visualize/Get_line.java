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

class Get_Line extends JPanel{
    
    private String Line = null;
	private String line = null;
	private int now_Line_num;
    private int solution[] = new int [30];
    private boolean flag = false;
    private int i = 0;
    private int x = 0;
    private int kazu = 0;
    private int oox,ooy;
    private int bbx = 900,bby = 400;
    private int waru = 0;
    private int min = 0;
    String[] msg_split;
    JFrame frame;
    java.awt.Rectangle r;


    public Get_Line(int mx, int my, int minmin){
        frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setBounds(mx+500, my, 300, 300);
        frame.setTitle("遷移解");
        frame.getContentPane().add(this);
        frame.setVisible(true);
        this.min = minmin;
    }

    private int getReaX( int xx){
        return oox + xx;
    }

    private int getReaY( int yy){
        return ooy - yy;
    }

	public String Get_now_Line(int n,int max, String Gname){
        
        try{
            
            FileInputStream gra = new FileInputStream(Gname);
			InputStreamReader phic = new InputStreamReader(gra, "EUC-JP");
			BufferedReader br = new BufferedReader(phic);

            this.now_Line_num = 0;
            this.i = 0;
            this.kazu = 0;

            line = br.readLine();
            line = null;

            if(n-1 < 30){
                while( (line = br.readLine()) != null){
                    if(kazu < n-1){
                        msg_split = line.split(",",0);
                        solution[this.i] = Integer.parseInt(msg_split[max+1]) - this.min;
                        System.out.println("solution[" + this.i + "]:" + solution[i]);
                        this.i++;
                    }
                    if(n == now_Line_num){
                        Line = line;
                        break;
                    }
                    kazu++;
                    this.now_Line_num ++;
                }
            }
            else{
                while( (line = br.readLine()) != null){
                    if(kazu > n-31 && kazu < n-1){
                        msg_split = line.split(",",0);
                        solution[this.i] = Integer.parseInt(msg_split[max+1]) - this.min;
                        System.out.println("solution[" + this.i + "]:" + solution[i]);
                        this.i++;
                    }
                    kazu++;
                    if(n == now_Line_num){
                        Line = line;
                        break;
                    }
                    this.now_Line_num ++;
                    if(this.i == 30){
                        break;
                    }
                }
            }

			br.close();
			phic.close();
			gra.close();

		}catch (IOException e){
		}

        this.flag = true;
        frame.repaint();
        
        return Line;
        
    }

    public void paintComponent(Graphics g){
        this.r = getBounds();
        this.oox = 0;
        this.ooy = r.height;
        this.bbx = r.width;
        this.bby = r.height;
        super.paintComponent(g);
        g.setColor(Color.red);
        this.x = 0;

        if(flag == true){
            for(int m=0;m<this.i-1;m++){ 
                g.drawLine(this.x,getReaY(this.solution[m] / 100),this.x+10,getReaY(this.solution[m+1] / 100) );
                this.x += 10;
            }
        }
    }
}
