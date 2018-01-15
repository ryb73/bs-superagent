open Js.Result;
open Option.Infix;

[@autoserialize]
type reqError = {
    method: string,
    status: int,
    url: string,
    message: string,
    stack: string
};

let serializeReqErrorOpt = fun
    | Some(v) => reqError__to_json(v)
    | None => Js.Json.boolean(Js.false_);

let deserializeReqErrorOpt = (json) =>
    switch (Js.Json.classify(json)) {
        | Js.Json.JSONFalse => Ok(None)
        | _ =>
            switch(reqError__from_json(json)) {
                | Ok(reqError) => Ok(Some(reqError))
                | Error(e) => Error(e)
            }
    };

let serializeJsonOpt = fun
    | Some(v) => v
    | None => Js.Json.null;

let deserializeJsonOpt = (json) =>
    switch (Js.Json.classify(json)) {
        | JSONNull => Ok(None)
        | _ => Ok(Some(json))
    };


[@autoserialize]
type result = {
    body: [@autoserialize.custom (serializeJsonOpt, deserializeJsonOpt, to_devtools)] option(Js.Json.t),
    clientError: bool,
    info: bool,
    notFound: bool,
    ok: bool,
    serverError: bool,
    status: int,
    statusCode: int,
    statusText: string,
    statusType: int,
    text: string,
    error: [@autoserialize.custom (serializeReqErrorOpt, deserializeReqErrorOpt, to_devtools)] option(reqError)
};

type contentType =
    | ApplicationJson;

type authorizationType =
    | Bearer;

type header =
    | ContentType(contentType)
    | Accept(contentType)
    | Authorization(authorizationType, string);

module Request = (M: { type t; }) => {
    [@bs.send] external withCredentials : M.t => M.t = "";
    [@bs.send.pipe : M.t] external query : Js.Dict.t(string) => M.t = "";

    [@bs.send] external _end : (M.t, (Js.nullable(string), Js.nullable(Js.Json.t)) => unit) => unit = "end";

    let end_ = (req) =>
        Js.Promise.make((~resolve, ~reject as _) =>
            _end(req, (err, resp) =>
                switch (Js.Nullable.to_opt(resp)) {
                    | Some(resp) =>
                        switch ((Js.Nullable.to_opt(err), result__from_json(resp))) {
                            | (Some(errMsg), Error(_)) => Js.Exn.raiseError(errMsg)
                            | (Some(_), Ok(_resp)) => Js.Exn.raiseError("ope")
                            | (None, Error(e)) => Js.Exn.raiseError("Error parsing response: " ++ (e |? ""))
                            | (_, Ok(resp)) => [@bs] resolve(resp)
                        }

                    | None =>
                        Js.Nullable.to_opt(err) |? "Unknown Error"
                            |> Js.Exn.raiseError
                }
            )
        );

    [@bs.send.pipe : M.t] external setHeaderCustom : (string, string) => M.t = "set";
    let setHeader = (header, req) =>
        switch header {
            | ContentType(v) =>
                let key = "Content-Type";
                switch v {
                    | ApplicationJson => setHeaderCustom(key, "application/json", req)
                }

            | Accept(v) =>
                let key = "Accept";
                switch v {
                    | ApplicationJson => setHeaderCustom(key, "application/json", req)
                }

            | Authorization(authType, credentials) =>
                let key = "Authorization";
                switch authType {
                    | Bearer => setHeaderCustom(key, "Bearer " ++ credentials, req)
                }
        };
};

module Post = {
    type t;

    include Request({ type nonrec t = t; });

    [@bs.send.pipe : t] external _send : string => t = "send";

    let send = (json, req) => req
        |> setHeader(ContentType(ApplicationJson))
        |> _send(Js.Json.stringify(json));
};

module Get = {
    type t;

    include Request({ type nonrec t = t; });
};

[@bs.module "superagent"] external get : string => Get.t = "";
[@bs.module "superagent"] external post : string => Post.t = "";
