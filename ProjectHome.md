# sync4cpp #

sync4cpp is a very small library introducing some markup for using mutexes in C++.
Mainly there are 4 markos (synchronize, sync\_here, sync\_xuse, sync\_use)
and 2 classes (synchronized, syncable) offering some more stylish ways to use mutexes and synchronism in C++.

Note that sync4cpp works primarily at compiletime and only little is done at runtime.
sync4cpp uses a lot of meta programming to reach its goals and hopefully the rest is optimized away by the compiler. The consequences are that only little functionality arises from sync4cpp! Instead of a traditional library, it is more a big _semantic_-lollipop offering stuff to work more easily and save with mutexes.

### Little examples ###

Working with boost::mutex:

```

boost::mutex m1;
boost::mutex m2;

{
  boost::lock_guard<boost::mutex> guard(m1);
  boost::lock_guard<boost::mutex> guard(m2);

  // Do some synced stuff here
  ...
}
```

Now use sync4cpp:

```

boost::mutex m1;
boost::mutex m2;

synchronize(m1, m2)
{
  // Do cool stuff here
  ...
}
```
See [synchronize](synchronize.md) for more informations.

You can also sync something in the current scope:

```

boost::mutex m1;
boost::mutex m2;

{
  sync_here(m1);
  sync_here(m2);

  // Do some synced stuff here
  ...
}
```
See [sync\_here](sync_here.md) for more informations.

Now lets do some cool stuff. You may know the _syncronized_ keyword in java. In cpp it is not that easy.
But luckily there is sync4cpp:

```

sync4cpp::synchronized<std::list<int> > IntList;

void thread_func()
{
  while(true)
  {
    sync_use(IntList);
    // IntList is now locked (ignore race conditions ;) )
    std::cout << IntList.front();
    IntList.push_back(IntList.front());
    IntList.pop_front();
  }
}

// Fill IntList, create a lot of threads here and call thread_func().
```
For more informations see [sync\_use](sync_use.md).

Last but not least we can make our own class syncable:
```
// inherit syncable private, because we dont want to be locked from the outside
class Log: private sync4cpp::syncable<boost::mutex>
{
public:
  void log(const std::string& msg)
  {
    sync_this();
    std::cout << msg;
  }
};

// using log(...) is threadsafe
```