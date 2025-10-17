module.exports = [
    {
        "type": "heading",
        "defaultValue": "App Configuration"
    },
    {
        "type": "text",
        "defaultValue": "Configure your app settings below."
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Temperature Settings"
            },
            {
                "type": "radiogroup",
                "messageKey": "ConfTemp",
                "defaultValue": "C",
                "label": "Temperature Unit",
                "options": [
                    {
                        "label": "Celsius (C)",
                        "value": "C"
                    },
                    {
                        "label": "Fahrenheit (F)",
                        "value": "F"
                    }
                ]
            },
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Terminal Settings"
            },
            {
                "type": "input",
                "messageKey": "ConfSymbol",
                "label": "Terminal Symbol",
                "defaultValue": "$",
                "attributes": {
                    "placeholder": "$",
                    "maxlength": 1
                }
            },
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    }
];