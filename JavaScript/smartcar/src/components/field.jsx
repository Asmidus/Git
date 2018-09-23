import React, { Component } from "react";

//this class takes an array of html5 input attributs and applies them to a single field
//input: array of attributes for <input> object
//output: the <input> object
class Field extends Component {
	state = {};

	render() {
		return this.validateProps() ? (
			this.printField()
		) : (
			<h4>Error initializing Field component props</h4>
		);
	}

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

	validateProps() {
		if (!("name" in this.props.attributes)) {
			return false;
		}
		return true;
	}

	generateLabel() {
		let lower = this.props.attributes.name;
		let upper = lower.charAt(0).toUpperCase() + lower.substr(1);
		return upper.replace("-", " ");
	}
}

export default Field;
