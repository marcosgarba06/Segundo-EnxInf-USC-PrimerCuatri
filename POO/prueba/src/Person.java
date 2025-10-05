public class Person {
    private String name; //Primera en mayuscula obxecto
    private int age; //Primera en minuscula primitiva

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(int age) {
        if (age < 0 || age > 120) {
            System.err.println("Invalid age");
        }else {
            this.age = age;
        }
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public static void main(String[] args) {

        Person p = new Person();

        p.setName("Maria"); //good
        p.setAge(25);       //Good
        p.setAge(-100);     //Will show error

        System.out.println(p.getName());
        System.out.println(p.getAge());
    }
}
