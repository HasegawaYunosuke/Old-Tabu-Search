import java.net.*;
import java.io.*;
import java.util.*;
import java.awt.event.* ;
import javax.swing.* ;
import java.awt.Graphics ;
import java.awt.Color ;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.Dimension;

class Real_Time extends JPanel implements Runnable{
    
    private Socket clntSock;
    private int no;
    boolean i=true;
    int j,k,l=10000,r=0;
    private int city_max = 0;
    private int[] abc = null;
    private int all_cost = 0;
    private String s1;
    private int city_index[] = null;
    private int city_point[] = null;
    private int bx = 0,by = 0;
    private int ox,oy;
    int b1,b2,b3,b4;
    private int judge = 1;
    private int best_solution = 0;
    private int first = 0, last = 0;
    private int Node_name = 0;

    JFrame frame;
    JLabel label;
    java.awt.Rectangle tan;
    Graphics g;

    public Real_Time(Socket clntSock, int no){
        this.clntSock = clntSock;
        this.no = no;
    }
    
    private int getRealX( int xx){
        return ox+xx;
    }

    private int getRealY( int yy){
        return oy-yy;
    }

    public void run(){
        
        try{

            if(this.no == 0){
                this.bx = 0;
                this.by = 0;
            }
            else if(this.no == 1){
                this.bx = 550;
                this.by = 0;
            }
            else if(this.no == 2){
                this.bx = 0;
                this.by = 520;
            }
            else if(this.no == 3){
                this.bx = 550;
                this.by = 520;
            }
            InputStream in = clntSock.getInputStream();
            DataInputStream ir1 = null;
            ir1 = new DataInputStream(in);
            while(in.available() == 0);
            
            b4 = ir1.read();
            b3 = ir1.read();
            b2 = ir1.read();
            b1 = ir1.read();
            
            city_max = (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) | ((b3 & 0xff) << 8) | (b4 & 0xff));
            
            city_point = new int[(city_max+1)*2];
            city_point[0] = city_max;
            
            for(r=1; r<(city_max+1)*2; r++){
                b4 = ir1.read();
                b3 = ir1.read();
                b2 = ir1.read();
                b1 = ir1.read();
                
                city_point[r] =  (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |((b3 & 0xff) << 8) | (b4 & 0xff));
            }

            int flag = 0;
            int a = 0;
            JFrame frame = null;

            city_index = new int[city_point[0]+3];

            do{
                DataInputStream ir2 = null;
                ir2 = new DataInputStream(in);

                if(city_index[0] == -1){
                    flag = 2;
                    i = false;
                    this.judge = 0;
                }

                while(in.available() == 0);
                
                for(r=0; r<(city_point[0]+1); r++){
                    b4 = ir2.read();
                    b3 = ir2.read();
                    b2 = ir2.read();
                    b1 = ir2.read();
                    
                    city_index[r] =  (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |((b3 & 0xff) << 8) | (b4 & 0xff));
                }

              if(city_index[0] == -1){
                  flag = 2;
                  i = false;
                  this.judge = 0;
              }
              
              b4 = ir2.read();
              b3 = ir2.read();
              b2 = ir2.read();
              b1 = ir2.read();
              
              this.all_cost = (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |((b3 & 0xff) << 8) | (b4 & 0xff));

              b4 = ir2.read();
              b3 = ir2.read();
              b2 = ir2.read();
              b1 = ir2.read();

              this.Node_name = (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |((b3 & 0xff) << 8) | (b4 & 0xff));

              if(flag == 0){
                  this.best_solution = this.all_cost;
              }

              if(city_index[0] == -1){
                  flag = 2;
                  i = false;
                  this.judge = 0;
              }
              
              if(flag == 0){
                  frame = new JFrame();
                  frame.getContentPane().add(this);
                  frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                  frame.setBounds(this.bx, this.by, 500, 500);
                  frame.setTitle("Node" + Node_name);
                  label = new JLabel();
                  frame.add(label,BorderLayout.NORTH);
                  frame.setVisible(true);
                  flag = 1;
                }
                else if(flag == 1){
                    frame.repaint();
                }
                else if(flag == 2){
                    frame.dispose();
                    ir1.close();
                    ir2.close();
                    in.close();
                    break;
                }
                
            }while(i);
            
            }catch (IOException e){
            }
                System.out.println("終了");
    }
    public void paintComponent(Graphics g){
        
        this.tan = getBounds();
        this.ox = 0;
        this.oy = tan.height;
        this.bx = tan.width;
        this.by = tan.height;
        super.paintComponent(g);
        
        label.setText("総距離" + this.all_cost);
        
        g.setColor(Color.RED);
        int m;
        int x1 = 0,y1 = 0;
        this.first = this.city_index[1];
        this.last = this.city_index[city_max];
        if(this.judge == 1){
            for(m=1; m<city_max; m++){
                x1 = this.city_index[m];
                y1 = this.city_index[m + 1];

                g.setColor(Color.BLUE);
                g.fillRect( this.city_point[x1*2] , getRealY( this.city_point[x1*2 + 1]),4,4 );
                g.setColor(Color.red);
                g.drawLine(( this.city_point[x1*2]), getRealY( this.city_point[x1*2 + 1]), ( this.city_point[y1*2]), getRealY( this.city_point[y1*2 + 1]) );
            }

            g.setColor(Color.BLUE);
            g.fillRect( this.city_point[last*2] , getRealY( this.city_point[last*2 + 1]),4,4 );
            g.fillRect( this.city_point[first*2] , getRealY( this.city_point[first*2 + 1]),4,4 );
            g.setColor(Color.RED);

            g.drawLine(( this.city_point[this.last*2]), getRealY( this.city_point[this.last*2 + 1]), ( this.city_point[this.first*2]),getRealY( this.city_point[this.first*2 + 1]) );
            g.dispose();
        }
    }
}
