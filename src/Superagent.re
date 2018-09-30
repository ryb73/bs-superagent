open Option.Infix;
open Reduice.Promise;

type request('a);
type acceptsBody;
type noAcceptBody;
type get = noAcceptBody;
type post = acceptsBody;
type put = acceptsBody;

[@decco]
type reqError = {
    method: string,
    status: int,
    url: string,
    message: string,
    stack: string
};

[@decco]
type resultWithError = {
    body: option(Js.Json.t),
    clientError: bool,
    info: bool,
    notFound: bool,
    ok: bool,
    serverError: bool,
    status: int,
    statusCode: int,
    statusText: option(string),
    statusType: int,
    text: string,
    error: [@decco.codec Decco.falseable] option(reqError)
};

[@decco]
type result = {
    body: option(Js.Json.t),
    clientError: bool,
    info: bool,
    notFound: bool,
    ok: bool,
    serverError: bool,
    status: int,
    statusCode: int,
    statusText: option(string),
    statusType: int,
    text: string,
};

type contentType =
    | ApplicationJson
    | ApplicationXWwwFormUrlencoded;

type authorizationType =
    | Bearer;

type header =
    | ContentType(contentType)
    | Accept(contentType)
    | Authorization(authorizationType, string);

exception ReqError(resultWithError);
exception ParseError(Js.Json.t, Decco.decodeError);

[@bs.send] external withCredentials : request('a) => request('a) = "";

[@bs.send.pipe : request('a)]
external setHeaderCustom : string => string => request('a) = "set";

let _getContentTypeString = fun
    | ApplicationJson => "application/json"
    | ApplicationXWwwFormUrlencoded => "application/x-www-form-urlencoded";

let setHeader = (header, req) =>
    switch header {
        | ContentType(v) => setHeaderCustom("Content-Type", _getContentTypeString(v), req)
        | Accept(v) => setHeaderCustom("Accept", _getContentTypeString(v), req)
        | Authorization(authType, credentials) =>
            let key = "Authorization";
            switch authType {
                | Bearer => setHeaderCustom(key, "Bearer " ++ credentials, req)
            }
    };

[@bs.send.pipe : request('a)]
external queryMultiple : Js.Dict.t(string) => request('a) = "query";

let query = (key, value, req) =>
    [| (key, value) |]
        |> Js.Dict.fromArray
        |> queryMultiple(_, req);

[@bs.send.pipe: request(acceptsBody)] external _send : string => request(post) = "send";
let send = (json, req) => req
    |> setHeader(ContentType(ApplicationJson))
    |> _send(Js.Json.stringify(json));

let sendKV = (key, value, req) =>
    [| (key, value) |]
        |> Js.Dict.fromArray
        |> Js.Json.object_
        |> send(_, req);

[@bs.send] external _end :
    request('a)
    => (Js.nullable(string) => Js.nullable(Js.Json.t) => unit)
    => unit = "end";

let _getResultWithoutError = ({ body, clientError, info, notFound, ok,
    serverError, status, statusCode, statusText, statusType, text, error: _ })
=> {
    body,
    clientError,
    info,
    notFound,
    ok,
    serverError,
    status,
    statusCode,
    statusText,
    statusType,
    text,
};

let end_ = (req) =>
    Reduice.Promise.make((~resolve, ~reject as _) =>
        _end(req, (err, resp) => resolve(. (err, resp)))
    )
    |> then_(((err, resp)) =>
        switch (Js.Nullable.toOption(resp)) {
            | Some(resp) =>
                switch ((Js.Nullable.toOption(err), resultWithError_decode(resp))) {
                    | (Some(errMsg), Error(_)) => Js.Exn.raiseError(errMsg)
                    | (Some(_), Ok(resp)) => raise(ReqError(resp))
                    | (None, Error(e)) => reject(ParseError(resp, e))
                    | (_, Ok({ error: Some(_) } as resp)) => raise(ReqError(resp))
                    | (_, Ok(resp)) => resolve(_getResultWithoutError(resp))
                }

            | None =>
                Js.Nullable.toOption(err) |? "Unknown Error"
                    |> Js.Exn.raiseError
        }
    );

[@bs.module "superagent"] external get : string => request(get) = "";
[@bs.module "superagent"] external post : string => request(post) = "";
[@bs.module "superagent"] external put : string => request(put) = "";
