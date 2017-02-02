import java.net.*;
import java.io.*;
import java.util.*;

public class ftclient {

	

	
    public static void main(String[] args) throws IOException 
    {

        try 
        {
            
            String response = "";
            String request = "";

            String hostName = "127.0.0.1";
            int portNumber = Integer.parseInt(args[0]);

            Socket socket = new Socket(hostName, portNumber);
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            
            Scanner keyboard = new Scanner(System.in);
            
            /*
            request = "client1";
            Functions.communicate(request, response, out, in);
            
            request = "client2";
            Functions.communicate(request, response, out, in);
            */
            
            
            String userInput;
            while(true)
            {
                
                
               
                System.out.println("Please enter command: ");
                
                userInput = keyboard.nextLine();
                
                request = userInput;
                 
                Functions.communicate(request, response, out, in);
                
                
                
                if(userInput.contains("-g"))
                {
                    
                    Functions.getFile();
                    
                    continue;
                    
                }
                
                else if (userInput.contains("-l"))
                {
                    
                    Functions.getDirectory();
                    
                    continue;
                    
                }
                
                else if (userInput.contains("quit"))
                {
                    
                    System.out.println("quit detected.");
                    
                    socket.close();
                    
                    System.exit(0);
                    
                }
                
                System.out.println("Server: " + response + "\n");
               
            }
            
          
           
        }

        catch (IOException e1)
        {

                e1.printStackTrace();

        }

        catch(Exception ex)
        {


        }

        finally {

        }
    }
        
}

class Functions
{
    
    public static void getallinsocket(String request, String response, PrintWriter out, BufferedReader in, Socket socket )
    {
        try{
            
            System.out.println(in.readLine());
            
            String line = null;
            line = in.readLine();
            while(line!=null)
            {
                
                //System.out.println("line is: " + line);
                System.out.println(line);
                
                if (line.equals("end"))
                {
                    
                    in.close();
                    out.close();
                    socket.close();
                    
                    System.exit(0);
                }
                
                line = in.readLine();
                
                //System.out.println("FLAG<<<1\n");
                
                
            }
            
            
            //System.out.println("FLAG1<\n");
            
            if (line == null)
            {
                
               // System.out.println("line is null\n");
            }
            else if (line != null)
            {
                //System.out.println("line is not null\n");
            }
            
            
            
            
            
        }
        
        
        catch (IOException e1)
        {
            
            e1.printStackTrace();
            
        }        
        
    }
    
    public static void communicate(String request, String response, PrintWriter out, BufferedReader in )
    {
        
        try{
         
            out.println(request);

            response = in.readLine();

            System.out.println("Response from server is: " + response);
        
        }
        
        
        catch (IOException e1)
        {
            
            e1.printStackTrace();
        }

        
    }
    
    public static void getFile()
    {
        
        Socket fileSocket = null;
        int PORT = 7000;
        
        try
        {

            fileSocket = new Socket("localhost", PORT);
        }
        catch(UnknownHostException uhe)
        {
            System.out.println("Unknown Host");
            fileSocket = null;

        }
        catch(IOException ioe)
        {

            System.out.println("Cant connect to server at 6000. Make sure it is running.");
            fileSocket = null;

        }  
        
        if (fileSocket == null)
            System.exit(-1);
        
        try
        {
            File file = new File("textReceived.txt");
            byte[] bytes = new byte[8192];
            FileInputStream fis = new FileInputStream(file);
            BufferedInputStream bis = new BufferedInputStream(fis);
            OutputStream outFile = fileSocket.getOutputStream();

            int count, file_size;

            while ((count = bis.read(bytes)) > 0) {
                System.out.println(count);
                outFile.write(bytes, 0 , count);

            }
            outFile.flush();
            outFile.close();
            fis.close();
            bis.close();

        }
        
        catch(IOException ioe)
        {

            System.out.println("Exception during communication. Server probably closed connection.");

        }
        finally
        {

            try
            {

                //close the socket before quitting
                fileSocket.close();

            }
            catch(Exception e)
            {

                e.printStackTrace();
            }
        }


    }
    
    public static void getDirectory()
    {
        try
        {
            
            
            
        
            String hostName = "127.0.0.1";
            int portNumber = 6000;
            
            Socket socketDir = new Socket(hostName, portNumber);
            PrintWriter outDir = new PrintWriter(socketDir.getOutputStream(), true);
            BufferedReader inDir = new BufferedReader(new InputStreamReader(socketDir.getInputStream()));
            BufferedReader stdInDir = new BufferedReader(new InputStreamReader(System.in));

            String request = "directory requested";
            String response = "";
            

            Functions.communicate(request, response, outDir, inDir);
            
            System.out.println("Server: Directory: \n" + response);
            
            Thread.sleep(1000);
            
            Functions.getallinsocket(request, response, outDir, inDir, socketDir);

        }
        
        catch (IOException e1)
        {

                e1.printStackTrace();

        }

        catch(Exception ex)
        {


        }

        finally {

        }
        
        
        

        
    }
}