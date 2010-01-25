import java.net.*;
import java.io.*;
import java.util.*;
import java.awt.event.* ;
import javax.swing.* ;
import java.awt.Graphics ;
import java.awt.Color ;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JScrollBar;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

class Auto_hand extends JPanel implements Runnable ,ActionListener, AdjustmentListener, KeyListener{
    
    private int city_index[] = null;
	private int Line_num;          
    private int city_max;
	private int city_point[] = null;
	private int Solution[] = null;
	private int now_Line_num = 1;
	private int speed_num = 350;
	private int jump_num;
    private int coma;
    private int numnum = 0;
    private int real_numnum = 1;
    private int min = 0;
	private String logname = null;
    private String Line = null;
	private boolean Auto_or_hand = true;
	private boolean ngya = true;
	private boolean ngya2 = true;
	private boolean flag = true;
	private boolean flag2 = true;
	private boolean flag3 = true;
	private boolean action1 = false;
	private boolean action2 = false;
	private boolean action3 = false;
	private boolean action4 = false;
	boolean i=true;
	private int ox,oy;
	private int bx = 0,by = 0;
	JFrame frame;
	JButton button1, button2, button3, button4, button5;
    JButton button6, button7;

	JLabel label;
	java.awt.Rectangle r;
	JScrollBar scroll;
    Get_Line a;

	public Auto_hand (int num, int max, int city_point[], String name,int com,int minmin){
        this.Line_num = num - 1;
		this.city_max = max;
		this.city_point = city_point;
        this.logname = name;
        this.coma = com;
        this.min = minmin;
	}

	private int getRealX( int xx){
        return ox+xx;
	}

	private int getRealY( int yy){
		return oy-yy;
	}

	public void run(){

        if(this.coma == 0){
            this.bx = 0;
            this.by = 0;
        }
        else if(this.coma == 1){
            this.bx = 850;
            this.by = 0;
        }
        else if(this.coma == 2){
            this.bx = 0;
            this.by = 520;
        }
        else if(this.coma == 3){
            this.bx = 850;
            this.by = 520;
        }

        this.city_index = new int[city_max + 2];
        a = new Get_Line(this.bx, this.by,this.min);
        Line = a.Get_now_Line(this.now_Line_num,this.city_max, this.logname);
        String[] Line_solution_path = Line.split(",", 0);

        for(int k=0; k<this.city_max+1; k++){
            this.city_index[k] = Integer.parseInt(Line_solution_path[k]);
        }

		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setBounds(this.bx, this.by, 500, 500 );
		frame.setTitle("描画:" + this.logname);

		button1 = new JButton("UP");
		button1.addActionListener(this);
		button1.setBounds(this.bx/25*18, this.by/10*8, 70, 50);
		button2 = new JButton("Down");
		button2.addActionListener(this);
		button2.setBounds(this.bx/25*5, this.by/10*8, 150, 50);
		frame.add(button1);
		frame.add(button2);

		button4 = new JButton("停止");
		button4.addActionListener(this);
		button4.setBounds(this.bx/25*13,this.by/10*8,70,50);
		button5 = new JButton("再生");
		button5.addActionListener(this);
		button5.setBounds(this.bx/25*8,this.by/10*8,70,50);
		frame.add(button4);
		frame.add(button5);

        button6 = new JButton("次");
        button6.addActionListener(this);
        button6.setBounds(this.bx/25*18, this.by/10*9, 50, 40);
        button7 = new JButton("前");
        button7.addActionListener(this);
        button7.setBounds(this.bx/25*15, this.by/10*9, 50, 40);
        frame.add(button6);
        frame.add(button7);
        button6.setEnabled (false);
        button7.setEnabled (false);


		label = new JLabel();
		frame.add(label,BorderLayout.NORTH);

		scroll = new JScrollBar(JScrollBar.HORIZONTAL, this.now_Line_num, 5, 0, this.Line_num);
		scroll.addAdjustmentListener(this);
		frame.add(scroll,BorderLayout.SOUTH);

		frame.getContentPane().add(this);
		frame.setVisible(true);

        addKeyListener(this);

        button4.setEnabled(false);

		do{
            try{
                Thread.sleep(30);
            }catch (InterruptedException e) {
			}
		}while(this.action3 == false);

        button4.setEnabled(true);

        for(;;){
            
            do{
                this.ngya = false;
                try{
                    Thread.sleep(10);
                }catch (InterruptedException e) {
				}
			}while(this.flag2 == false);

            if(this.numnum > 0){
                this.now_Line_num = this.numnum*5 + this.now_Line_num - 1;
            }
            if(this.numnum < 0){
                this.now_Line_num = this.numnum*5 + this.now_Line_num - 1;
                if(this.now_Line_num < 0){
                    this.now_Line_num = 1;
                    this.numnum = 0;
                }
            }
			if(this.Auto_or_hand == true){

                if(this.now_Line_num > this.Line_num){
                    this.now_Line_num = this.Line_num;
                }

                Line = a.Get_now_Line(this.now_Line_num,this.city_max, this.logname);
                Line_solution_path = Line.split(",", 0);
                    
                for(int j=0; j<this.city_max+1; j++){
                    this.city_index[j] = Integer.parseInt(Line_solution_path[j]);
                }
                    
                frame.repaint();
                    
                try {
                    Thread.sleep(this.speed_num);
                }catch (InterruptedException e) {
                }
                    
                this.now_Line_num++;
                this.ngya = true;
            }
			else if(this.Auto_or_hand == false){
                
                if(this.ngya = true){
                    if(this.ngya2 == true){
                        this.now_Line_num--;
					}

					if(this.action1 == true){
                        this.now_Line_num++;
                    }
                    
                    if(this.action2 == true){
                        
                        if(this.now_Line_num > 1){
                            if(this.numnum > 0){
                                if(this.now_Line_num - this.numnum*10 < 1){
                                    this.now_Line_num = 1;
                                }
                                else{
                                    this.now_Line_num = this.now_Line_num - this.numnum*10 + 1;
                                }
                            }
                            else{
                                this.now_Line_num--;
                            }
						}
						else{
                        }
					}
                    
                    if(this.now_Line_num > this.Line_num){
                            this.now_Line_num = this.Line_num;
                    }
                
                    Line = a.Get_now_Line(this.now_Line_num,this.city_max, this.logname);
				    Line_solution_path = Line.split(",", 0);
				    for(int j=0; j<this.city_max+1; j++){
                        this.city_index[j] = Integer.parseInt(Line_solution_path[j]);
				    }
				    frame.repaint();
				}
                
                this.action1 = false;
				this.action2 = false;
				this.action4 = false;
				this.flag = false;
				this.flag2 = false;
				this.ngya2 = false;
			}

			this.scroll.setValues(this.now_Line_num, 5, 0, this.Line_num);

		}
		}
        
        public void adjustmentValueChanged(AdjustmentEvent e) {
            JScrollBar scroll = (JScrollBar)e.getSource();
            this.now_Line_num = scroll.getValue();
            if(this.now_Line_num == 0){
                this.now_Line_num = 1;
            }
            Line = a.Get_now_Line(this.now_Line_num,this.city_max, this.logname);
            String[] Line_solution_path2 = Line.split(",", 0);
            for(int j=0; j<this.city_max+1; j++){
                this.city_index[j] = Integer.parseInt(Line_solution_path2[j]);
            }

            frame.repaint();
		}

        public void keyPressed(KeyEvent ke){

            int keycode = ke.getKeyCode();
            if(keycode == KeyEvent.VK_ENTER){
                if(this.Auto_or_hand == true){
                    this.speed_num = this.speed_num/2;
                }
                if(this.Auto_or_hand == false){
                    this.action1 = true;
                    this.ngya = false;
                    this.flag = true;
                    this.flag2 = true;
                }
            }
            if(keycode == KeyEvent.VK_LEFT){
                if(this.Auto_or_hand == true){
                    this.speed_num = this.speed_num*2;
                }
                if(this.Auto_or_hand == false){
                    this.action2 = true;
                    this.flag = true;
                    this.flag2 = true;
                    this.ngya = false;
                }
            }
            if(ke.getKeyCode() == ke.VK_UP){
            }
            if(ke.getKeyCode() == ke.VK_DOWN){
            }
        }

        public void keyReleased(KeyEvent ke){
        }

        public void keyTyped(KeyEvent ke){
        }

		public void actionPerformed(ActionEvent ae){
            
			if (ae.getSource().equals(button1)) {
                if(this.Auto_or_hand == true){
                    if(this.numnum < 0){
                        this.numnum = 0;
                    }
                    this.numnum += 1;
				}
				if(this.Auto_or_hand == false){
                    this.numnum += 1;
				}
			}
								
			if (ae.getSource().equals(button2)) {
				if(this.Auto_or_hand == true){
                    if(this.numnum > 0){
                        this.numnum = 0;
                    }
                    this.numnum -= 1;

				}
				if(this.Auto_or_hand == false){
                    this.numnum -= 1;
				}
			}

			if (ae.getSource().equals(button4)) {
					this.action3 = false;
					this.flag = false;
					this.flag2 = false;
					this.ngya2 = true;
					this.Auto_or_hand = false;
					this.button1.setText("＋");
					this.button2.setText("ー");
                    this.numnum = 0;
					frame.setTitle("手動モード:" + this.logname);
					button4.setEnabled(false);
                    button6.setEnabled(true);
                    button7.setEnabled(true);
			}

			if (ae.getSource().equals(button5)) {
				    this.action3 = true;
					this.flag = true;
					this.flag2 = true;
					this.Auto_or_hand = true;
					this.button1.setText("UP");
					this.button2.setText("Down");
                    this.numnum = 0;
					frame.setTitle("自動モード:" + this.logname);
					button4.setEnabled(true);
                    button6.setEnabled(false);
                    button7.setEnabled(false);
			}
            
            if (ae.getSource().equals(button6)) {
                if(this.Auto_or_hand == false){
                    this.action1 = true;
                    this.ngya = false;
                    this.flag = true;
                    this.flag2 = true;
                }
            }

            if (ae.getSource().equals(button7)) {
                if(this.Auto_or_hand == false){
                    this.action2 = true;
                    this.flag = true;
                    this.flag2 = true;
                    this.ngya = false;
                }
            }
		}

		public void paintComponent(Graphics g){
            
            this.r = getBounds();
			this.ox = 0;
			this.oy = r.height/2+100;
			this.bx = r.width;
			this.by = r.height;
			super.paintComponent(g);
			label.setText("解" + this.now_Line_num + "つ目");
			g.setColor(Color.red);
			button1.setBounds(this.bx/25*18, this.by/10*8, 70, 50);
			button2.setBounds(this.bx/25*3, this.by/10*8, 80, 50);
			button4.setBounds(this.bx/25*13,this.by/10*8,70,50);
			button5.setBounds(this.bx/25*8,this.by/10*8,70,50);
            button6.setBounds(this.bx/25*18,this.by-30,50,40);
            button7.setBounds(this.bx/25*15,this.by-30,50,40);
			int first = 0, last = 0;
			int m;
			int x1 = 0, y1 = 0;
			first = this.city_index[1];
			last = this.city_index[this.city_max];
			for(m=1; m<this.city_max; m++){
                x1 = this.city_index[m];
				y1 = this.city_index[m + 1];
                g.setColor(Color.BLUE);
                g.fillRect( this.city_point[x1*2] , getRealY( this.city_point[x1*2 + 1]),4,4 );

                g.setColor(Color.red);
				g.drawLine(getRealX( this.city_point[x1*2]), getRealY( this.city_point[x1*2 + 1]), getRealX( this.city_point[y1*2]), getRealY( this.city_point[y1*2 + 1] ));
			}

            g.setColor(Color.BLUE);
            g.fillRect( this.city_point[last*2] , getRealY( this.city_point[last*2 + 1]),4,4 );
            g.fillRect( this.city_point[first*2] , getRealY( this.city_point[first*2 + 1]),4,4 );
            g.setColor(Color.RED);

            g.drawLine(getRealX( this.city_point[last*2]), getRealY( this.city_point[last*2 + 1]), getRealX( this.city_point[first*2]), getRealY( this.city_point[first*2 + 1]) );

			g.dispose();
		}
}
