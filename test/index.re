{
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
        |> send (Js.Json.object_(Js.Dict.fromList([("var", Js.Json.string("var"))])))
        |> end_
        |> then_((result) => {
            Js.log(result);
            resolve()
        })
        |> catch((err) => {
            Js.log(err);
            resolve ();
        });
};

{
    module SA = Superagent.Make(Bluebird);
    open SA;
    open Bluebird;

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
        |> send (Js.Json.object_(Js.Dict.fromList([("var", Js.Json.string("var"))])))
        |> end_
        |> then_((result) => {
            Js.log(result);
            resolve()
        })
        |> catch((err) => {
            Js.log(err);
            resolve ();
        });
};
