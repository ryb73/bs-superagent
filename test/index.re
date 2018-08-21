{
    open Superagent;
    open Js.Promise;

    get("api/")
        |> withCredentials
        |> query(Js.Dict.fromList([("derp", "hello"), ("lmao", "123")]))
        |> end_
        |> then_ ((resp) => {
            switch (resp.error) {
                | Some(error) => Js.log("Error: " ++ error.message)
                | None =>
                    switch (resp.body) {
                        | None => Js.log("No Body")
                        | Some(body) => Js.log(body)
                    }
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
        |> query(Js.Dict.fromList([("derp", "hello"), ("lmao", "123")]))
        |> end_
        |> then_ ((resp) => {
            switch (resp.error) {
                | Some(error) => Js.log("Error: " ++ error.message)
                | None =>
                    switch (resp.body) {
                        | None => Js.log("No Body")
                        | Some(body) => Js.log(body)
                    }
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
