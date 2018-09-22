import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import registerServiceWorker from "./registerServiceWorker";
import "bootstrap/dist/css/bootstrap.css";
import Explorer from "./components/explorer";

ReactDOM.render(
	<div>
		<Explorer
			title={"Get users"}
			url={"https://jsonplaceholder.typicode.com/users"}
			method={"GET"}
		/>
		<Explorer
			title={"Add new user"}
			url={"https://jsonplaceholder.typicode.com/users"}
			method={"POST"}
			body={[
				{
					name: "email",
					type: "email",
					max: 24,
					min: 3
				},
				{
					name: "full-name",
					type: "text",
					placeholder: "John Doe",
					required: true
				},
				{
					name: "phone",
					type: "tel",
					pattern: "[0-9]{3}-[0-9]{3}-[0-9]{4}"
				}
			]}
		/>
		<Explorer
			title={"Edit user"}
			url={"https://jsonplaceholder.typicode.com/users"}
			method={"PUT"}
			body={[
				{
					name: "id",
					required: true
				},
				{
					name: "email",
					type: "email",
					max: 24,
					min: 3
				},
				{
					name: "full-name",
					type: "text",
					placeholder: "John Doe",
					required: true
				},
				{
					name: "phone",
					type: "tel",
					pattern: "[0-9]{3}-[0-9]{3}-[0-9]{4}"
				}
			]}
		/>
		<Explorer
			title={"Delete a user"}
			url={"https://jsonplaceholder.typicode.com/users"}
			method={"DELETE"}
			body={[
				{
					name: "id"
				}
			]}
		/>
	</div>,
	document.getElementById("root")
);
registerServiceWorker();
