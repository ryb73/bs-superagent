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
    url: option(string),
    message: string,
    stack: string,
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
    error: option(reqError),
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

type contentType = ApplicationJson | ApplicationXWwwFormUrlencoded;
type authorizationType = Bearer;
type header =
    | ContentType(contentType)
    | Accept(contentType)
    | Authorization(authorizationType, string);

exception ReqError(resultWithError);
exception ParseError(Js.Json.t, Decco.decodeError);

let withCredentials: request('a) => request('a);
let setHeaderCustom: (string, string, request('a)) => request('a);
let setHeader: (header, request('a)) => request('a);
let queryMultiple: (Js.Dict.t(string), request('a)) => request('a);
let query: (Js.Dict.key, string, request('a)) => request('a);
let type_: ([ `form | `json ], request(acceptsBody)) => request(acceptsBody);
let send: (string, request(acceptsBody)) => request(acceptsBody);
let sendDict: (Js.Dict.t(Js.Json.t), request(acceptsBody)) => request(acceptsBody);
let sendArray: (array(Js.Json.t), request(acceptsBody)) => request(acceptsBody);
let sendKV: (Js.Dict.key, Js.Json.t, request(acceptsBody)) => request(acceptsBody);
let field: (string, Js.Json.t, request(acceptsBody)) => request(acceptsBody);
let end_: request('a) => Js.Promise.t(result);
let get: string => request(get);
let post: string => request(post);
let put: string => request(put);
