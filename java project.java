import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.Vector;

public class ShoppingCartApp extends JFrame {
    Connection conn;
    JTable productTable, cartTable;
    DefaultTableModel productModel, cartModel;
    JLabel totalLabel;
    double total = 0;

    public ShoppingCartApp() {
        // DB Connection
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/shoppingdb", "root", "yourpassword");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "DB Error: " + e.getMessage());
            System.exit(0);
        }

        setTitle("Online Shopping Cart");
        setSize(700, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Product Table
        productModel = new DefaultTableModel(new String[]{"ID", "Name", "Price"}, 0);
        productTable = new JTable(productModel);
        loadProducts();

        // Cart Table
        cartModel = new DefaultTableModel(new String[]{"Name", "Price"}, 0);
        cartTable = new JTable(cartModel);
        totalLabel = new JLabel("Total: $0.00");

        // Buttons
        JButton addToCartBtn = new JButton("Add to Cart");
        addToCartBtn.addActionListener(e -> addToCart());

        // Layout
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(new JScrollPane(productTable), BorderLayout.CENTER);
        topPanel.add(addToCartBtn, BorderLayout.SOUTH);

        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.add(new JScrollPane(cartTable), BorderLayout.CENTER);
        bottomPanel.add(totalLabel, BorderLayout.SOUTH);

        add(topPanel, BorderLayout.NORTH);
        add(bottomPanel, BorderLayout.CENTER);
    }

    void loadProducts() {
        try {
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery("SELECT * FROM products");
            while (rs.next()) {
                Vector<Object> row = new Vector<>();
                row.add(rs.getInt("id"));
                row.add(rs.getString("name"));
                row.add(rs.getDouble("price"));
                productModel.addRow(row);
            }
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "Error loading products: " + e.getMessage());
        }
    }

    void addToCart() {
        int selected = productTable.getSelectedRow();
        if (selected != -1) {
            String name = productModel.getValueAt(selected, 1).toString();
            double price = Double.parseDouble(productModel.getValueAt(selected, 2).toString());
            cartModel.addRow(new Object[]{name, price});
            total += price;
            totalLabel.setText("Total: $" + String.format("%.2f", total));
        } else {
            JOptionPane.showMessageDialog(this, "Select a product first!");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new ShoppingCartApp().setVisible(true));
    }
}
