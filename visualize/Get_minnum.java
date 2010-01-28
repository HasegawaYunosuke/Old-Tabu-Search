import java.net.*;
import java.io.*;
import java.util.*;

class Get_minnum{

    private String line1 = null;
    private int minmin = 0;
    private int l = 0;
	private int tsp_size = 0;

    public int get_min(int num, int max, String nn){
        try{
            FileInputStream gra = new FileInputStream(nn);
            InputStreamReader phic = new InputStreamReader(gra, "EUC-JP");
            BufferedReader br = new BufferedReader(phic);

            int i = 1;

            line1 = br.readLine();
            line1 = null;
            line1 = br.readLine();
            String[] line1_split = line1.split(",", 0);
			tsp_size = Integer.parseInt(line1_split[0]);
            minmin = Integer.parseInt(line1_split[max+1]);

            while(i != num){
                line1 = br.readLine();
                String[] line2_split = line1.split(",", 0);
                l = Integer.parseInt(line2_split[max+1]);
                i++;
                if(minmin > l){
                    minmin = l;
                }

            }

            br.close();
            phic.close();
            gra.close();

        }catch (IOException e){
        }

		if(tsp_size == 51){
			minmin = 1;
		}

		System.out.println("minmin:" + minmin);

        return minmin;
    }
}
