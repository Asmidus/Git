import React, { Component } from "react";

class Field extends Component {
	state = {};

	render() {
		let label = this.generateLabel();
		return (
			<div>
				<label>{label}</label>
				<br />
				<input {...this.props.attributes} />
				<br />
			</div>
		);
	}

	generateLabel() {
		let lower = this.props.attributes.name;
		let upper = lower.charAt(0).toUpperCase() + lower.substr(1);
		return upper.replace("-", " ");
	}
}

export default Field;
