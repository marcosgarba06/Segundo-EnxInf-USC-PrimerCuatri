package partida;
import java.util.Random;


public class Dado {
    // Atributo del dado
    private int valor;

    // Método para simular una tirada
    public int hacerTirada() {
        Random rand = new Random();
        this.valor = rand.nextInt(6) + 1; //Se hace asi porque random
        return valor;
    }

    //Setter para
    public void setValor(int valor) {
        if (valor < 1 || valor > 6) {
           System.out.println("Error: valor invalido. Los valores de un dado son entre 1 y 6.");
        }else{
            this.valor = valor;
        }
    }

    // Getter para el valor del dado
    public int getValor() {
        return valor;
    }

}
