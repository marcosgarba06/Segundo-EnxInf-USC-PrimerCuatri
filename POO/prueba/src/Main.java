public class Main {
    public static void main(String[] args) {
        Car myCar = new Car();

        myCar.setColor("red");
        myCar.setSpeed(50);
        System.out.println(myCar.getColor());
        System.out.println(myCar.getSpeed());

        Car myCar2 = new Car("Green", 120);
        System.out.println(myCar2.getColor());
        System.out.println(myCar2.getSpeed());
    }
}