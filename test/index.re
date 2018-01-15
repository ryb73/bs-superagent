open Superagent;
open Js.Promise;

get("api/")
    |> Get.withCredentials
    |> Get.query(Js.Dict.fromList([("derp", "hello"), ("lmao", "123")]))
    |> Get.end_
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
    |> Post.withCredentials
    |> Post.send (Js.Json.object_(Js.Dict.fromList([("var", Js.Json.string("var"))])))
    |> Post.end_
    |> then_((result) => {
        Js.log(result);
        resolve()
    })
    |> catch((err) => {
        Js.log(err);
        resolve ();
    });

/* open Js.Result;

let serializeOption = fun
    | Some(i) => Js.Json.number(float_of_int(i))
    | _ => Js.Json.null;

let deserializeOption = (json) =>
    switch (Js.Json.classify(json)) {
        | Js.Json.JSONNull =>  Ok(None)
        | Js.Json.JSONNumber(f) => Ok(Some(int_of_float(f)))
        | _ => Error(None)
    };

type j = {
    blah: int,
    blah2: [@autoserialize.custom (serializeOption, deserializeOption)] option(int)
};

let j = {blah: 1, blah2: Some(-1) };
Js.log(j);
let json = j__to_json(j);
Js.log(json);
switch (j__from_json(json)) {
    | Ok(from) => Js.log(from);
    | Error(e) => Js.log2("error:", e)
} */