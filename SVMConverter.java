public class SVMConverter {
    static String hex = "adac 0550 0000 0000 1000 0000 0000 0000 0100 0000 4415 000e 4418 000e 4417 0046";

    public static void main(String[] args) {
        String result = "";
        boolean isFirst = true;
        int counter = 0;
        for (int i = 0; i < hex.length(); ++i) {
            if (hex.charAt(i) == ' ' || hex.charAt(i) == '\n') {continue;}
            if (isFirst) {
                result += "0x" + String.valueOf(hex.charAt(i));
                isFirst = false;
            } else {
                result += String.valueOf(hex.charAt(i)) + ", ";
                isFirst = true;
                counter += 1;
            }
        }
        System.out.println(result);
        System.out.println("length: " + counter);
    }
}
