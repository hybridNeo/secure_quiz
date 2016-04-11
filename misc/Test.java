import java.io.*;
public class Test{
	public static String fixedLengthString(String string, int length) {
	    return String.format("%1$"+length+ "s", string);
	}
	public static void main(String[] args){
		String q1 = fixedLengthString("Which is the first operating system?",120);
		//System.out.println(fixedLengthString(q1,120));
		String a1 = fixedLengthString("d13bbbd92b83ddaad994a12bd9d20dfba5fff139",40);
	try{
		//System.out.println(fixedLengthString(a1,40));
		ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("binarytest"));
		//out.writeChars(q1+a1+q1+a1);
		String f = q1+a1+q1+a1;
		out.writeObject(f);
		// out.write(data);
	}catch(IOException e){
		e.printStackTrace();
	}
	}
}
