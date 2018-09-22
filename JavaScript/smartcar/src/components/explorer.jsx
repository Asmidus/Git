import React, { Component } from "react";
import FieldList from "./fieldList";

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
		//handle GET request
		if (this.props.method === "GET") {
			fetch(this.props.url)
				.then(response => response.json())
				.then(json => this.setState({ response: JSON.stringify(json) }));
			return;
		}
		//handle request that requires input (and DELETE since there is no harm sending an empty body)
		else {
			let formatData = {};
			data.forEach(function(value, key) {
				formatData[key] = value;
			});
			//send request and receive answer
			fetch(this.props.url, {
				method: this.props.method,
				body: JSON.stringify(formatData),
				headers: {
					"Content-type": "application/json; charset=UTF-8"
				}
			})
				.then(response => response.json())
				.then(json => this.setState({ response: JSON.stringify(json) }));
		}
	}

	render() {
		return (
			<div style={{ paddingLeft: 25 }}>
				{this.printHeader()}
				<h2>Body</h2>
				<form onSubmit={this.handleSubmit}>
					<FieldList body={this.props.body} />
					<br style={{ lineHeight: 4 }} />
					<input type="submit" value="Send Request" />
				</form>
				{this.printResponse()}
			</div>
		);
	}

	printHeader() {
		let headers = [];
		if (this.props.title) {
			headers.push(<h1>{this.props.title}</h1>);
		}
		if (this.props.method) {
			headers.push(<h4 style={{ color: "red" }}>{this.props.method}</h4>);
		}
		if (this.props.url) {
			headers.push(<h2>Base URL</h2>);
			headers.push(<h4>{this.props.url}</h4>);
		}
		return headers;
	}

	printResponse() {
		return (
			<textarea
				cols="100"
				rows="5"
				readOnly={true}
				value={this.state.response}
			/>
		);
	}
}

export default Explorer;
