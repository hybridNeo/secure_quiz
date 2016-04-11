import java.io.*;
public class Test{
	public static String fixedLengthString(String string, int length) {
	    return String.format("%1$"+length+ "s", string);
	}
	public static void main(String[] args){
		String q1 = "Which is the first operating system?";
		//System.out.println(fixedLengthString(q1,120));
		String a1 = "d13bbbd92b83ddaad994a12bd9d20dfba5fff139";
	try{
		//System.out.println(fixedLengthString(a1,40));
		PrintWriter writer = new PrintWriter("ques", "UTF-8");
		//out.writeChars(q1+a1+q1+a1);
		String f = q1+"\n"+a1+"\n"+q1+"\n"+a1;
		writer.println(f);
		writer.close();
		// out.write(data);
	}catch(IOException e){
		e.printStackTrace();
	}
	}
}
