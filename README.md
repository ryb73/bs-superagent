# bs-superagent

## What is it?
BuckleScript bindings for [SuperAgent](https://visionmedia.github.io/superagent/).

## What state is it in?
Incomplete. The published version may lag behind the GitHub repo – feel free to file a bug if this is the case.

## How do I install it?
```
npm i @ryb73/bs-superagent
```

## How do I use it?
Example:
```reasonml
open Superagent;
open Js.Promise;

get("api/")
|> withCredentials
|> query("key", "value")
|> end_
|> then_((resp) => {
    switch (resp.body) {
        | None => Js.log("No Body")
        | Some(body) => Js.Json.decodeObject(body) |> doSomething
    };

    resolve();
});
```

See [`test/index.re`](test/index.re) and https://visionmedia.github.io/superagent/ for more.
