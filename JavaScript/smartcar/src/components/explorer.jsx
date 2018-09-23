import React, { Component } from "react";
import FieldList from "./fieldList";

class Explorer extends Component {
	state = {
		response: ""
	};

	render() {
		return (
			<div style={{ paddingLeft: 25 }}>
				{this.printHeader()}
				{this.printForm()}
				{this.printResponse()}
			</div>
		);
	}

	//handle the submission of the request (when the user clicks the send request button)
	handleSubmit = event => {
		event.preventDefault();
		const data = new FormData(event.target);
		//handle GET request and receieve response
		if (this.props.method === "GET") {
			fetch(this.props.url)
				.then(response => response.json())
				.then(json =>
					this.setState({ response: JSON.stringify(json, null, 2) })
				);
		}
		//handle request that requires input (and DELETE since there is no harm sending an empty body)
		else {
			//format the input data for sending
			let formatData = {};
			data.forEach(function(value, key) {
				formatData[key] = value;
			});
			//send request and receive response
			fetch(this.props.url, {
				method: this.props.method,
				body: JSON.stringify(formatData),
				headers: {
					"Content-type": "application/json; charset=UTF-8"
				}
			})
				.then(response => response.json())
				.then(json =>
					this.setState({ response: JSON.stringify(json, null, 2) })
				);
		}
	};

	//print the header for the explorer
	printHeader() {
		let headers = [];
		//manually add each defined prop input value to the header array if it exists
		if (this.props.title) {
			headers.push(<h1>{this.props.title}</h1>);
		}
		if (this.props.method) {
			headers.push(<h3 style={{ color: "red" }}>{this.props.method}</h3>);
		}
		if (this.props.url) {
			headers.push(<h2>Base URL</h2>);
			headers.push(<h4>{this.props.url}</h4>);
		}
		//add the body header as it is common for every instance of this component
		headers.push(<h2>Body</h2>);
		return headers;
	}

	//print the response box for the explorer
	printResponse() {
		return (
			<textarea
				cols="100"
				rows="8"
				readOnly={true}
				value={this.state.response}
			/>
		);
	}

	//print the input form that should be filled out for the explorer
	printForm() {
		return (
			//create a form with a variable number of fields to be filled out and submitted
			<form onSubmit={this.handleSubmit}>
				<FieldList body={this.props.body} />
				<br style={{ lineHeight: 4 }} />
				<input type="submit" value="Send Request" />
				<br style={{ lineHeight: 2 }} />
			</form>
		);
	}
}

export default Explorer;
