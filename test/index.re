open Superagent;
open Js.Promise;

get("api/")
    |> withCredentials
    |> queryMultiple(Js.Dict.fromList([("derp", "hello"), ("lmao", "123")]))
    |> query("and", "another")
    |> end_
    |> then_ ((resp) => {
        switch (resp.body) {
            | None => Js.log("No Body")
            | Some(body) => Js.log(body)
        };

        resolve();
    })
    |> catch((err) => {
        Js.log(err);
        resolve ();
    });

post("api/post")
    |> withCredentials
    |> field("var", Js.Json.string("var"))
    |> end_
    |> then_((result) => {
        Js.log(result);
        resolve()
    })
    |> catch((err) => {
        Js.log(err);
        resolve ();
    });

put("api/put")
    |> withCredentials
    |> send("hey=no")
    |> end_
    |> then_((result) => {
        Js.log(result);
        resolve()
    })
    |> catch((err) => {
        Js.log(err);
        resolve ();
    });
