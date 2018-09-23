import React, { Component } from "react";
import FieldList from "./fieldList";

//this component receives a title, url, method, and body (any of which may be emp)
class Explorer extends Component {
	state = {
		response: ""
	};

	render() {
		return this.validateProps() === true ? ( //if input validation passes
			this.printExplorer() //print the component
		) : (
			//otherwise print the source of the error (or)
			<h1>Error initializing explorer component props</h1>
		);
	}

	//check the props passed as input to see if they are of valid types and values, returns true if everything checks out
	validateProps() {
		if (
			!this.props.method ||
			typeof this.props.method != "string" ||
			(this.props.method !== "GET" &&
				this.props.method !== "POST" &&
				this.props.method !== "PUT" &&
				this.props.method !== "DELETE")
		)
			return false;

		if (!this.props.url || typeof this.props.url != "string") return false;

		if (
			(this.props.method === "PUT" || this.props.method === "POST") &&
			(!this.props.body || typeof this.props.body != "object")
		) {
			return false;
		}

		return true;
	}

	//handle the submission of the request (when the user clicks the send request button)
	handleSubmit = event => {
		event.preventDefault();
		const data = new FormData(event.target);
		//handle GET request and receieve response
		if (this.props.url) {
			if (this.props.method === "GET") {
				fetch(this.props.url)
					.then(response => response.json())
					.then(json =>
						this.setState({ response: JSON.stringify(json, null, 2) })
					);
			}
			//handle request that requires input (and DELETE since there is no harm sending an empty body)
			else if (this.props.body || this.props.method === "DELETE") {
				//only process the submission if it has a body for input or the method is DELETE
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
			} else {
				//no body for input detected when it was required
				this.setState({ response: "Error, no body for input detected" });
			}
		} else {
			//no url was input
			this.setState({ response: "Error, no URL detected" });
		}
	};

	printExplorer() {
		return (
			<div style={{ paddingLeft: 25 }}>
				{this.printHeader()}
				{this.printForm()}
				{this.printResponse()}
			</div>
		);
	}

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
