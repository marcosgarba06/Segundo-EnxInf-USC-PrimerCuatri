public class Car {

    private String color;
    private int speed;


    public void setColor(String color) {
        this.color = color;
    }

    public void setSpeed(int speed) {
        this.speed = speed;
    }

    public String getColor() {
        return color;
    }

    public int getSpeed() {
        return speed;
    }

    public Car(){
        this.color = "Unknown";
        this.speed = 0;
    }
    public Car(String color, int speed){ //Constructor con parametros
        setColor(color);
        setSpeed(speed);
    }
}
