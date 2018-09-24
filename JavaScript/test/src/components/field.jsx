import React, { Component } from "react";

//this class takes an array of html5 input attributs and applies them to a single field
//input:array of attributes for <input> object
//output:the <input> object
class Field extends Component {
	state = {};
	//if the props pass validation, print the component, otherwise replace with error message
	render() {
		return this.validateProps() ? (
			this.printField()
		) : (
			<h4>Error initializing Field component props</h4>
		);
	}

	//generate the label from the name prop and print the component
	printField() {
		let label = this.generateLabel();
		return (
			<div>
				<label>{label}</label>
				<br />
				<input {...this.props.attributes} />
				<br style={{ lineHeight: 2 }} />
			</div>
		);
	}

	//make sure that the input prop is an object that contains a "name" attribute
	validateProps() {
		if (
			typeof this.props.attributes !== "object" ||
			!("name" in this.props.attributes)
		) {
			return false;
		}
		return true;
	}

	//take the name attribute and turn it into a more appropriate title for the text box by removing dashes and capitalizing the first letter
	generateLabel() {
		let lower = this.props.attributes.name;
		let upper = lower.charAt(0).toUpperCase() + lower.substr(1);
		return upper.replace("-", " ");
	}
}

export default Field;
