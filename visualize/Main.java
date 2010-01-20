/*

 ***READ ME***

    I'll show you to how to compile.
    So, type to comand-line according to following.

        $ javac Main.java RealTime.java Record.java

    and I'll show you to how to action.
    Please type to comand-line according to following.

    RealTime Mode   $ java Main 10001 100
    Record Mode     $ java Main 10001 200

    If compile and action are terminated by something error, please complain to writer.

*/


import java.net.*;
import java.io.*;

public class Main{

    public static void main(String[] args) throws IOException {
	
    if(args.length != 2)
		throw new IllegalArgumentException("Parameter(s): <Port> or <Mode>");

	int echoServPort = Integer.parseInt(args[0]);
	int Mode = Integer.parseInt(args[1]);

	if(Mode != 100 && Mode != 200)
		throw new IllegalArgumentException("exit");
		
	ServerSocket servSock = new ServerSocket(echoServPort);

	int no = 0;

	if(Mode == 100){
        for(;;){
            try{
                Socket clntSock = servSock.accept();
				Real_Time protocol = new Real_Time(clntSock, no);
				Thread thread = new Thread(protocol);
				thread.start();
                no++;

                if(no == 4){
                    no = 0;
                }
				} catch (IOException e) {
				}
		}
	}
	if(Mode == 200){
		for(;;){
			try{
				Socket clntSock = servSock.accept();
				Record record = new Record(clntSock,no);
				Thread thread = new Thread(record);
				thread.start();
				no++;
				} catch (IOException e) {
				}
		}
	}
	}
}
