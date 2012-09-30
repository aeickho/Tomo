namespace tomo
{
  namespace slicing
  {
    template<class State> 
    class Procedure
    {
      typedef Action<State> Action;

      std::vector<Action> operator()(Polygon& _input, const vector<Polygon>& _output);
    };

    class Filling : 
  }
}

