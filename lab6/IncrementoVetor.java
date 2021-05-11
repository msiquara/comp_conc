class V {
    //recurso compartilhado
    private int[] v;
    //construtor
    public V(int[] vet) { 
        this.v = vet; 
    }

    public synchronized void inc(int i) { 
        this.v[i]++; 
    }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {    
    private int id; //identificador da thread
    private int n; //numero de threads
    //objeto compartilhado com outras threads
    V v;

    //construtor
    public T(int tid, V v, int N) { 
        this.id = tid; 
        this.v = v;
        this.n = N;
    }

    //metodo main da thread
    public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        for (int i = this.id; i < IncrementoVetor.size; i += this.n) {
            this.v.inc(i);  
            //System.out.println("Thread " + this.id + " somou 1 a " + i); //checa o que as threads estao fazendo
        }

        System.out.println("Thread " + this.id + " terminou!"); 
    }
}

//classe da aplicacao
class IncrementoVetor {
    static final int N = 2; //quantidade de thread
    static final int size = 8; //tamanho do array

    public static void main (String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];
        int[] vet = new int[size];

        //preenche array
        for (int i = 0; i < size; i++){
            vet[i] = i;
        }   

        System.out.println("Array inicial: ");

        for (int i = 0; i < size; i++){
            System.out.printf(vet[i] + " ");
        }  

        System.out.println("\n");

        //cria uma instancia do recurso compartilhado entre as threads
        V v = new V(vet);

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new T(i, v, N);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println("\nArray final: ");

        for (int i = 0; i < size; i++){
            System.out.printf(vet[i] + " ");
        }  

        System.out.println("");
    }
}
