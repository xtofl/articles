# No, that's Not an Int

## The Pattern

### The API

    template<typename T>
    class Matrix {
    public:
      Matrix(int rows, int cols);
      T operator()(int row, int col) const;
      T& operator()(int row, int col);
      ... fill(T), clear(), size(), rows(), cols(), ...
    private:
      class Impl;
      std::unique_ptr<Impl> impl;
    };
    
How's that for an interface?  Not bad, is it?  Looks familiar!  After all, it's just a wrapper around a chunk of memory.

### The Client Code

    CMatrix<float> samples(100, 3); // 100 samples of red, green, blue
    for(auto i = 0; i != 100; ++i) {
      auto sample = image.next_pixel();
      samples(i, 0) = sample.red();
      samples(i, 1) = sample.green();
      samples(i, 2) = sample.blue();
    }

## No!

I boldly claim that if your code should reflect your problem domain.  Only sparsely should you allow technology constraints to influence this idea.

This way you can easily spot problems like so:

    namespace index {
      struct Row { int value; };
      struct Column { int value: };
    }
    
    template<typename T>
    class Matrix {
    public:
      Matrix(index::Row rows, index::Column cols);
      T operator()(index::Row row, index::Column col) const;
      T& operator()(index::Row row, index::Column col);
      ... fill(T), clear(), size(), rows(), cols(), ...
    private:
      class Impl;
      std::unique_ptr<Impl> impl;
    };
    
