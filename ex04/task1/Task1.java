import java.util.ArrayList;
import java.util.List;

public class Task1 {

  public static void main(String[] args) {
      Person p1 = new Person("Klaus", "Test", 46);
      Room r = new Room(1, 10);

      r.iterateOverPeople();
      r.enter(p1);
      r.iterateOverPeople();
      r.exit(p1);
      r.iterateOverPeople();
  }

}

class Person {

  private String firstName;
  private String lastName;
  private int age;

  public Person() {}

  public Person(String firstName, String lastName, int age){
    this.firstName = firstName;
    this.lastName = lastName;
    this.age = age;
  }

  public String getInfo() {
    return this.firstName+ " " + this.lastName+ " ("+ this.age +")";
  }
}

class Room {

  private int id;
  // Java has no unsigned integers
  private int limit;
  private List<Person> people = new ArrayList<>();

  public Room(int id, int limit) {
    if(limit < 0) {
      // should throw exception
      this.limit = 0;
    } else {
      this.id = id;
      this.limit = limit;
    }
  }

  public int countPeople(){
    return this.people.size();
  }

  public boolean isPersonInRoom(Person p){
    return this.people.contains(p);
  }

  public void iterateOverPeople(){
    if(this.people.isEmpty()){
      System.out.println("Room empty");
      return;
    }
    System.out.println("People in room:");
    for(Person p : this.people){
  		System.out.print(p.getInfo() + ", ");
  	}
    System.out.println("");
  }

  public boolean enter(Person p){
    if(this.people.size() < this.limit)
    {
      this.people.add(p);
      System.out.println(p.getInfo() + " has entered the room");
      return true;
    }
    System.out.println("Room is full! " + p.getInfo() + " cannot enter");
    return false;
  }

  public void exit(Person p){
    this.people.remove(p);
  }

}
