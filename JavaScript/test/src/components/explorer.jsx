import React, { Component } from "react";
import Field from "./field";

class Explorer extends Component {
	state = {
		response: ""
	};

	constructor() {
		super();
		this.handleSubmit = this.handleSubmit.bind(this);
	}

	handleSubmit(event) {
		event.preventDefault();
		const data = new FormData(event.target);
		if (this.props.method === "GET") {
			fetch(this.props.url)
				.then(response => response.json())
				.then(json => console.log(json));
			return;
		} else {
			let formatData = {};
			let path = "";
			data.forEach(function(value, key) {
				formatData[key] = value;
			});
			if (this.props.method === "PUT" || this.props.method === "DELETE") {
				if (data.get("id")) {
					path = "/" + data.get("id");
				} else {
					console.log("Error, need an id to put or delete");
				}
			}
			fetch(this.props.url + path, {
				method: this.props.method,
				body: JSON.stringify(formatData),
				headers: {
					"Content-type": "application/json; charset=UTF-8"
				}
			})
				.then(response => response.json())
				.then(json => console.log(json));
		}
	}

	render() {
		console.log("props", this.props);
		return (
			<div>
				<h1>{this.props.title}</h1>
				<h4>{this.props.method}</h4>
				<h2>Base URL</h2>
				<h4>{this.props.url}</h4>
				<h2>Body</h2>
				<form onSubmit={this.handleSubmit}>
					{this.printFields()}
					<br style={{ lineHeight: 4 }} />
					<input type="submit" value="Send Request" />
				</form>
			</div>
		);
	}

	printFields() {
		if (this.props.body) {
			let fields = [];
			for (let i = 0; i < this.props.body.length; i++) {
				fields.push(<Field attributes={this.props.body[i]} />);
			}
			return fields;
		}
		return;
	}
}

export default Explorer;
