#OOP is dead, long live OOP
##Blog post summary
https://www.gamedev.net/blogs/entry/2265481-oop-is-dead-long-live-oop/

- OO ... object oriented
- OOP ... OO programming
- OOD ... OO design
- ECS ... Entity Component Systems

This post written by Hodgman is a reaction to a [publication](http://aras-p.info/texts/files/2018Academy%20-%20ECS-DoD.pdf) of  Aras Pranckevičius where he shows some terrible OOP code and then the relation model as a great alternative solution.

Hodgman criticizes that it is not a fair comparison because the OOP code presented "violates all sorts of core OO rules". He fixes the OOD violations in the code and gets to a result with similar performance improvement as the "ECS"-code presented by Aras. Additionaly it uses also less RAM and requires less lines of code.


> Code that uses OOP language features, but does not follow OOD design rules **is not OO code**

###4 tools of OOP

Hodgman desribes the "4 tools of OOP":
- Abstraction
- Encapsulation
- Polymorphism
- Inheritance

These are powerful tools which can be used to solve problems but, he says, it is important to know when you should use them and not simply learn how to use them.

###Five core rules
- **Single responsibility principle**: if class *A* has two responsibilities, create class *B* and *C* to handle each of them and then compose *A* out of *B* and *C*.
- **Open/closed principle**: put parts likely to change into concrete classes and build interfaces around the parts unlikely to change
- **Liskov substitution principle**: any algorithm that works on a interface should continue to work for all implementations <a name="lsp-rule"></a>
- **Interface segregation principle**: Keep interfaces as small as possible
- **Dependency inversion principle**: instead of having implementations communicating directly, decouple them by formalizing their communication interface as a third class.
- **Composite reuse principle**: Composition is the right default, inheritance should be reserved for use when it is absolutely required.

###Inheritance types
The interfaces' purpose is to hide the implementation. There are two types of inheritance:
- **interface inheritance**: abstract base classes with pure-virtual functions
- **implementation inheritance**: occurs any time a base classes contains anything besides pure-virtual functions. This should be treated as a bit of a code smell.


###Terrible OOP education

####About hierarchies/inheritance
Let's say you have a university app that contains a directory of Students and Staff. We can make a Person base class, and then a Student class and a Staff class that inherit from Person!


NO, NO, NO! If we don't know what kind of algorithms are going to be operating on *Students* and *Staff* then it's downright irresponsible to dive in and start designing class hierarchies.

> You have to know the algorithms **and** the data first

#### About inheritance type
Let's say you have a shape class. We could also have squares and rectangles as sub-classes. Should we have square is-a rectangle, or rectangle is-a square?

Good example to distinguish between implementation-inheritance and interface-inheritance.

#####Attempt 1: implementation-inheritance

~~~
struct Square { int width; };
struct Rectangle : Square { int height; };
~~~

By inheriting from square, the rectangle class (according to the LSP) must obey the rules of square's interface. **Any algorithm that works correctly with a square, must also work correctly with a rectangle.**

~~~
std::vector<Square*> shapes;
int area = 0;
for(auto s : shapes)
  area += s->width * s->width;
~~~

This will work correctly for squares (producing the sum of their areas), but will not work for rectangles. Therefore, Rectangle violates the [Liskov substitution principle](#lsp-rule) expained above.

#####Attempt 2: interface-inheritance

If you're using the interface-inheritance mindset, then neither Square or Rectangle will inherit from each other. The interface for a square and rectangle are actually different, and one is not a super-set of the other.

#####Solution: Implementation-inheritance
OOD says that composition is the right way to go!

~~~
struct Shape
{
  virtual int area() const = 0;
};

struct Square : public virtual Shape
{
  virtual int area() const {
    return width * width;
  };
  int width;
};

struct Rectangle : private Square, public virtual Shape
{
  virtual int area() const {
    return width * height;
  };
  int height;
};
~~~

> your OOP class told you what inheritance was. Your missing OOD class should have told you not to use it 99% of the time!

After this introduction Hodgman shows how Aras [code](https://github.com/aras-p/dod-playground/blob/3529f232510c95f53112bbfff87df6bbc6aa1fae/source/game.cpp) can be improved with OOP. This code is re-implementing the existing language feature of composition as a runtime library instead of a language feature.

###Aras' code

This is an "Entity/Component" framework (sometimes confusingly called an "Entity/Component system") -- but completely different to an "Entity Component System" framework. It formalizes several "EC" rules:

- the game will be built out of featureless "Entities" called ```GameObject```, which themselves are composed out of ```Component```.
- ```GameObject``` fulfill the *service locator pattern* -- they can be queried for a child component by type.
- Components know to which ```GameObject``` they belong to -- they can locate sibling components by querying their parent ```GameObject```.
- Composition may only be one level deep (Components may not own child components, GameObjects may not own child GameObjects).
- A ```GameObject``` may only have one component of each type (some frameworks enforced this, others did not).
- Every component (probably) changes over time in some unspecified way - so the interface includes "virtual void Update".
- GameObjects belong to a scene, which can perform queries over all GameObjects (and thus also over all Components).

####Evaluation

1. ```GameObject::GetComponent``` uses dynamic_cast. This is a code smell and a indication for a violation of the [Liskov substitution principle](#lsp-rule)

2. ```GameObject``` is kind of ok if you imagine that it's fulfilling the service locator pattern... but going beyond OOD critique for a moment, this pattern creates implicit links between parts of the project

3. ```Component``` is a violation of the [Single responsibility principle](#lsp-rule) because its interface (```virtual void Update(time)```) is too broad. The use of "virtual void Update" is pervasive within game development.

4. Even though the goal of the ```Component``` class is to enable composition, it's doing so via inheritance, which is a [Composite reuse principle](#lsp-rule) violation.

###C++ code

Before:  https://github.com/hodgman/dod-playground/blob/3529f232510c95f53112bbfff87df6bbc6aa1fae/source/game.cpp

After: https://github.com/hodgman/dod-playground/blob/f42290d0217d700dea2ed002f2f3b1dc45e8c27c/source/game.cpp

##Summary

Hodgman first improves the understandability of the code which was completely obfuscating both the flow of control and the flow of data within the game.

Aras had a base class ``Component`` from which ```PositionComponent```, ```SpriteComponent```, ```MoveComponent``` inherited. But this forces him them to use templates. Hodgman removed this implementation-inheritance and added constructors:

~~~
struct PositionComponent : public Component
{
  float x, y;
};
~~~

becomes:

~~~
struct PositionComponent
{
    float x, y;

    PositionComponent(float x, float y)
		: x(x)
		, y(y)
    {
	}
};
~~~

He removes the three hard-coded blocks of code that resemble a template/prefab -- code that creates a ```GameObject``` containing specific Component types, and replace it with three C++ classes:

~~~
// create "world bounds" object
WorldBoundsComponent bounds;
{
    EntityID go = s_Objects.AddEntity("bounds");
    s_Objects.m_WorldBounds[go].xMin = -80.0f;
    s_Objects.m_WorldBounds[go].xMax =  80.0f;
    s_Objects.m_WorldBounds[go].yMin = -50.0f;
    s_Objects.m_WorldBounds[go].yMax =  50.0f;
    bounds = s_Objects.m_WorldBounds[go];
    s_Objects.m_Flags[go] |= Entities::kFlagWorldBounds;
    s_MoveSystem.SetBounds(go);
}

// create regular objects that move
for (auto i = 0; i < kObjectCount; ++i)
{
    EntityID go = s_Objects.AddEntity("object");

    // position it within world bounds
    s_Objects.m_Positions[go].x =
        RandomFloat(bounds.xMin, bounds.xMax);
    s_Objects.m_Positions[go].y =
        RandomFloat(bounds.yMin, bounds.yMax);
    s_Objects.m_Flags[go] |= Entities::kFlagPosition;

    // setup a sprite for it (random sprite index from first 5),
    // and initial white color
    s_Objects.m_Sprites[go].colorR = 1.0f;
    s_Objects.m_Sprites[go].colorG = 1.0f;
    s_Objects.m_Sprites[go].colorB = 1.0f;
    s_Objects.m_Sprites[go].spriteIndex = rand() % 5;
    s_Objects.m_Sprites[go].scale = 1.0f;
    s_Objects.m_Flags[go] |= Entities::kFlagSprite;

    // make it move
    s_Objects.m_Moves[go].Initialize(0.5f, 0.7f);
    s_Objects.m_Flags[go] |= Entities::kFlagMove;
    s_MoveSystem.AddObjectToSystem(go);

    // make it avoid the bubble things,
    // by adding to the avoidance system
    s_AvoidanceSystem.AddObjectToSystem(go);
}

// create objects that should be avoided
for (auto i = 0; i < kAvoidCount; ++i)
{
    EntityID go = s_Objects.AddEntity("toavoid");

    // position it in small area near center of world bounds
    s_Objects.m_Positions[go].x =
          RandomFloat(bounds.xMin, bounds.xMax) * 0.2f;
    s_Objects.m_Positions[go].y =
          RandomFloat(bounds.yMin, bounds.yMax) * 0.2f;
    s_Objects.m_Flags[go] |= Entities::kFlagPosition;

    // setup a sprite for it (6th one), and a random color
    s_Objects.m_Sprites[go].colorR = RandomFloat(0.5f, 1.0f);
    s_Objects.m_Sprites[go].colorG = RandomFloat(0.5f, 1.0f);
    s_Objects.m_Sprites[go].colorB = RandomFloat(0.5f, 1.0f);
    s_Objects.m_Sprites[go].spriteIndex = 5;
    s_Objects.m_Sprites[go].scale = 2.0f;
    s_Objects.m_Flags[go] |= Entities::kFlagSprite;

    // make it move, slowly
    s_Objects.m_Moves[go].Initialize(0.1f, 0.2f);
    s_Objects.m_Flags[go] |= Entities::kFlagMove;
    s_MoveSystem.AddObjectToSystem(go);

    // add to avoidance this as "Avoid This" object
    s_AvoidanceSystem.AddAvoidThisObjectToSystem(go, 1.3f);
}

~~~

becomes:

~~~
WorldBounds bounds;
std::vector<AvoidThis> avoidThis;
std::vector<RegularObject> regularObject;
~~~

because Hodgman uses classes. He therefore also don't need Templates for distinguishing between the different Components, for example this function:

~~~
// get a component of type T,
// or null if it does not exist on this game object
template <typename T>
T* GetComponent ()    
{
  for ( auto i : m_Components)
  {
    T* c = dynamic_cast<T*>(i);
    if (c != nullptr)
      return c;
  }

  return nullptr;
}
~~~
