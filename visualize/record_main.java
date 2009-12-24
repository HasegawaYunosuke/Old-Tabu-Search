/*

    *** READ ME ***

    I'll show you to how to compile.
    So, type to comand-line according to following.

        $ javac record_main.java Auto_hand.java Get_line.java Get_minnum.java

    and I'll show you to how to action.
    Please type to comand-line according to following.

    $ java record_main [name.log]

    If compile and action are terminated by something error, please complain to writer.

*/
import java.net.*;
import java.io.*;

public class record_main{

    public static void main(String[] args) throws IOException {
        
        if(args.length > 4){
            System.exit(1);
        }
        
        System.out.println("Review System ON!");
        
        for(int command = 0; command<args.length; command++){

            FileInputStream gra = new FileInputStream(args[command]);
            InputStreamReader phic = new InputStreamReader(gra, "EUC-JP");
            BufferedReader br = new BufferedReader(phic);
            
            int num = 0;
            int minmin = 0;
            int min_sol = 500000;
            int max = 0;
            int i = 0;
            int ore = 0;
            String line1 = null;
            String[] msg_split;
            String[] line_split;
            
            String msg = br.readLine();
            msg_split = msg.split(",", 0);
            max = Integer.parseInt(msg_split[0]);

            int point[] = new int[max*2+2];
            
            for(int k=0; k<(max)*2+2; k++){
                point[k] = Integer.parseInt(msg_split[k]);
                }

            while( ( line1 = br.readLine() ) != null ){
                num++;
            }

            br.close();
            phic.close();
            gra.close();

            Get_minnum s = new Get_minnum(); 
            minmin = s.get_min(num, max, args[command]);

            Auto_hand auto_hand = new Auto_hand(num, max, point, args[command], command, minmin);
            Thread thread3 = new Thread(auto_hand);
            thread3.start();
        }
    }
}
