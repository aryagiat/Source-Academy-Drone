import java.io.FileInputStream;

public class Main {
  public static void main(String[] argv) throws Exception {
    FileInputStream fin = new FileInputStream("prog.svm");

    int len;
    byte data[] = new byte[16];

    // Read bytes until EOF is encountered.
    System.out.print("[");
    do {
      len = fin.read(data);
      for (int j = 0; j < len; j++)
        System.out.printf("0x%02X, ", data[j]);
    } while (len != -1);
    System.out.print("]");
  }
}

   
