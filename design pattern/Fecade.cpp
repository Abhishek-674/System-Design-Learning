class TV {
  public:
  void on() {}
};

class SoundSystem {
 public:
  void start() {}
};

class Lights {
   public:
  void dim() {}
};
// Fecade class will have the object of all the classes
class MovieFacade {
  TV* tv = new TV();
  SoundSystem* sound = new SoundSystem();
  Lights* lights = new Lights();
  public:
  void watchMovie() {
    lights->dim();
    tv->on();
    sound->start();
  }
};

int main()
{
    MovieFacade* facade = new MovieFacade();
    facade->watchMovie();

    return 1;
}

/*
Problem it Solves
You have many classes / complex logic
Client code becomes hard to understand
Client tightly couples with multiple classes
Facade hides complexity

Client ───▶ Facade
              |
              | has a
              v
     SubsystemA   SubsystemB   SubsystemC

/