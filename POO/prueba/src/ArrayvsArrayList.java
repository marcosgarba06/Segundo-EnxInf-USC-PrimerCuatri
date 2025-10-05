import java.util.ArrayList;
import java.util.Arrays;

public class ArrayvsArrayList {


    public static void main(String[] args) {
        ArrayList<String> friendsArrayList2 = new ArrayList<>();
        friendsArrayList2.add("Pablo");
        friendsArrayList2.add("Xuli");
        friendsArrayList2.add("PutoPorco");

        System.out.println(friendsArrayList2);

        friendsArrayList2.remove("PutoPorco");
        friendsArrayList2.add("Nerea");

        System.out.println(friendsArrayList2);

        ArrayList<String> friendsArrayList3 = new ArrayList<>(Arrays.asList("Pablo", "Xuli", "Nerea"));
        System.out.println(friendsArrayList3);
    }
}


