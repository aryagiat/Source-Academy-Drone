import java.io.FileInputStream;

public class Main {
  public static void main(String[] argv) throws Exception {
    FileInputStream fin = new FileInputStream("program.svm");

    int len;
    byte data[] = new byte[1];

    // Read bytes until EOF is encountered.
    System.out.print("{");
    do {
      len = fin.read(data);
      // by right, only 1 byte is read each time in SVM file 
      for (int j = 0; j < len; j++)
        System.out.printf("0x%02X, ", data[j]);
    } while (len != -1);
    System.out.print("}");
  }
  
}

   
