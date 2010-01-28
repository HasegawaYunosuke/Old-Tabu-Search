import java.net.*;
import java.io.*;
import java.util.*;
import java.awt.event.*;
import java.awt.Container;

class Record implements Runnable{
    
    private Socket clntSock;
    private int city_index[] = null;
    private int city_point[] = null;
    boolean i = true;
    int r = 0;
    private int value = 0;
    private int city_max = 0;
    int waki = 0;
    int b1,b2,b3,b4;
    private int u = 0;
    private int[] abc = null;
    private int all_cost = 0;
    private int Node_name = 0;
    
    public Record(Socket clntSock, int value){
        this.clntSock = clntSock;
        this.value = value;
    }
    
    public void run(){
        
        try{
            InputStream in = clntSock.getInputStream();
            DataInputStream ir1 = null;
            ir1 = new DataInputStream(in);
            while(in.available() == 0);
            
            b4 = ir1.read();
            b3 = ir1.read();
            b2 = ir1.read();
            b1 = ir1.read();
            
            city_max = (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |
                        ((b3 & 0xff) << 8) | (b4 & 0xff));
            
            abc = new int[(city_max+1)*2];
            abc[0] = city_max;
            
            for(r=1; r<(city_max+1)*2; r++){
                b4 = ir1.read();
                b3 = ir1.read();
                b2 = ir1.read();
                b1 = ir1.read();
                
                abc[r] =  (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |
                            ((b3 & 0xff) << 8) | (b4 & 0xff));
            }
            
            String[] str = new String[(city_max+1)*2*2];
            int kkk = 0;
            int lll = 0;
            
            for(lll=0; lll<(city_max+1)*2; lll++){
                str[kkk] = Integer.toString(abc[lll]);
                kkk++;
                str[kkk] = ",";
                kkk++;
            }

            DataInputStream ir2 = null;
            ir2 = new DataInputStream(in);
            while(in.available() == 0);

            int abcd[] = new int[abc[0]+3];
            String[] str1 = new String[(abc[0]+3)*2];

            for(r=0; r<abc[0]+1; r++){
                b4 = ir2.read();
                b3 = ir2.read();
                b2 = ir2.read();
                b1 = ir2.read();

                abcd[r] =  (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |
                ((b3 & 0xff) << 8) | (b4 & 0xff));
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

            for(lll=0; lll<abc[0]+1; lll++){
                str1[waki] = Integer.toString(abcd[lll]);
                waki++;
                str1[waki] = ",";
                waki++;
            }

            str1[waki] = Integer.toString(this.all_cost);
            waki++;
            str1[waki] = ",";
            waki++;
            str1[waki] = Integer.toString(this.Node_name);
            waki++;

            Calendar cal1 = Calendar.getInstance();
            int year = cal1.get(Calendar.YEAR);
            int month = cal1.get(Calendar.MONTH) + 1;
            int day = cal1.get(Calendar.DATE);
            int hour = cal1.get(Calendar.HOUR_OF_DAY);
            int minute = cal1.get(Calendar.MINUTE);
            String filename = String.valueOf(year) + String.valueOf(month) + String.valueOf(day) + "_" + String.valueOf(hour) + "." + String.valueOf(minute) + ".Node" + this.Node_name;
            
            FileOutputStream fos = new FileOutputStream(filename + ".log");
            OutputStreamWriter osw = new OutputStreamWriter(fos , "MS932");
            BufferedWriter bw = new BufferedWriter(osw);

            for(r=0; r<kkk; r++){
                bw.write(str[r]);
            }
            bw.newLine();

            for(r=0; r<waki; r++){
                bw.write(str1[r]);
            }

            bw.newLine();

            int a = 0;
            
            do{
                waki = 0;
                
                for(r=0; r<abc[0]+1; r++){
                    b4 = ir2.read();
                    b3 = ir2.read();
                    b2 = ir2.read();
                    b1 = ir2.read();
                    
                    abcd[r] =  (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) |
                                ((b3 & 0xff) << 8) | (b4 & 0xff));
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
                
                if(abcd[0] == -1){
                    bw.close();
                    osw.close();
                    fos.close();
                    break;
                }
                
                for(lll=0; lll<abc[0]+1; lll++){
                    str1[waki] = Integer.toString(abcd[lll]);
                    waki++;
                    str1[waki] = ",";
                    waki++;
                }

				if(abcd[0] == -1){
					bw.close();
					osw.close();
					fos.close();
					break; 
				}

                str1[waki] = Integer.toString(this.all_cost);
                waki++;
                str1[waki] = ",";
                waki++;
                str1[waki] = Integer.toString(this.Node_name);
                waki++;

				if(abcd[0] == -1){ 
					bw.close();
					osw.close();
					fos.close(); 
					fos.close();
				}

                for(r=0; r<waki; r++){
                    bw.write(str1[r]);
                }
                bw.newLine();
                
            }while(i);

            System.out.println("終了");
            
        }catch (IOException e){
        }
    }
}
